/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/mail.c — threads that exchange any value while they run: the
 * language's `spawn`, `t.wait()`, `mailbox()`, `m.send(v)`, `m.receive(...)`
 * and `m.close()` (spec § 3, § 11, § 13; feature 2647; panel 254, its R1 to
 * R6, provisional; design.md Part 7.13).
 *
 * EVERY VALUE CROSSES WHOLE, AS A COPY, IN A FLAT ENCODING (panel 254's R1,
 * the compiler-engineer's Q2.2). The sending thread writes the value into one
 * block of the runtime's own (`HeroCross`, `hero_malloc_raw`, weighed by
 * neither leak counter, as `parts/spawn_str.c`'s box is), and the receiving
 * thread rebuilds it from that block into blocks it allocates itself. So every
 * block a program's value owns is born and given back on one thread, Part
 * 7.13's heaps; only the transport block and the queue object are made on one
 * thread and given back on another. The walk is a family of functions of two
 * shapes, `HeroPutFn` and `HeroTakeFn`, passed beside the descriptor and never
 * inside it: this file writes the `str`'s, the failure's, an array's, a map's,
 * a mailbox's, a thread's and a C handle's; the compiler writes one of each
 * per type a program crosses (`emit/crossing.hero`), and the raw bytes of a
 * type that owns nothing. The descriptor keeps its five members.
 *
 * A MAILBOX IS A QUEUE EVERY COPY REACHES (spec § 3's address sentence; S5).
 * `HeroMail` counts the program's values that reach it, atomically, and goes
 * with the last of them, the values still in it rebuilt and dropped on the
 * thread that drops it. No copy-on-write ever tests that count: every write to
 * the queue is under its own lock. A send never waits; it fails `closed` once
 * the mailbox is closed and `stopped` once it has answered `stopped`, the
 * value then left as it was (S6). A receive takes the oldest value, waiting at
 * most `within_ms`, -1 for no limit (S7). A close is for every copy, and
 * closing again does nothing (S8).
 *
 * THE STOP DRAINS, THEN HOLDS (panel 254's R4, the critic's behaviour 3).
 * Once the program's stop is asked (`hero_net.h`'s, by a signal it caught or
 * by `hero_net_ask_stop`), a receive still takes what is queued; one that
 * finds its mailbox empty answers `stopped`, and that mailbox answers
 * `stopped` from then on, a send to it failing `stopped`. `closed` comes
 * before `stopped`. The layer tells this file of the stop by one line at the
 * end of `hero_net_nudge_all` (R6), on a thread of its own and never inside a
 * signal handler; this file names no symbol of the layer, and a program that
 * links no member of it cannot be stopped, so its receives never answer
 * `stopped`.
 *
 * A QUEUE NAMED BY NUMBER (panel 256's R6 to R8, provisional). `m.number()`
 * gives the layer an `i64` for the queue, never the queue: a slot of this
 * file's table and the slot's life, so the number names nothing once the last
 * copy is gone, and never the next queue of that slot. By number the layer asks
 * whether a receive would wait (`hero_mail_ready`) and registers a watch, a
 * function of its own handed by pointer, rung when the queue goes from empty
 * to one value, at its close, at the stop and at its end
 * (`platform/hero_layer_mail.h`). So the layer's code runs on this file's
 * threads while the runtime names none of its symbols, and a program that
 * links no member links none of it.
 *
 * THE ORDER OF THE LOCKS, one order everywhere: the layer's lock, then the
 * queues' list lock, then one queue's lock, then the handle set's or the
 * thread table's. No queue operation ever takes the layer's lock, and a watch's
 * ring, called under a queue's lock or the list's, takes no lock at all.
 *
 * A HANDLE BELONGS TO THE THREAD THAT HOLDS IT (panel 254's R3). A C handle's
 * life (`parts/alloc.c`) and a thread's slot (`parts/spawn.c`) record the
 * thread holding them, by this runtime's own thread number: a send hands every
 * one a value holds over, before the value is queued, and empties the sender's
 * cell; a receive, and a wait's answer, take them over on the receiving
 * thread. Reaching C through a handle, or waiting for a thread, on a thread
 * that does not hold it aborts, naming the call.
 *
 * NO QUEUE HOLDS ITSELF (panel 254's R5). The checker refuses a mailbox type
 * whose values may hold that mailbox; and when `main` returns, after every
 * thread was waited for, a queue still alive can only be held by a copy of
 * itself inside a queue, so it is told, by count, as a hole in that rule. */

#if !defined(_WIN32)
/* Named here rather than inherited, `parts/spawn.c`'s reason: a file that leans
 * on another file's includes works until that file grows a configuration. */
#include <pthread.h>
#include <time.h>
#endif

/* The place a refusal names, or the bare words where the program gave none. */
_Noreturn static void hero_mail_panic(const char *msg, const HeroPlace *where) {
    if (where != NULL) hero_panic_at(msg, where->path, where->name, where->line, where->column);
    hero_panic(msg);
}

/* -- the crossing walk ------------------------------------------------------- */

/* Room for `n` more bytes, the block grown geometrically. */
static void hero_cross_room(HeroCross *b, size_t n) {
    if (n > SIZE_MAX - b->len) hero_panic("a value too large to cross between threads");
    if (b->len + n <= b->cap) return;
    size_t cap = b->cap == 0 ? 64 : b->cap;
    while (cap < b->len + n) {
        if (cap > SIZE_MAX / 2) hero_panic("a value too large to cross between threads");
        cap *= 2;
    }
    unsigned char *grown = (unsigned char *)hero_malloc_raw(cap);
    if (b->len > 0) memcpy(grown, b->bytes, b->len);
    if (b->bytes != NULL) hero_free_raw(b->bytes);
    b->bytes = grown;
    b->cap = cap;
}

static void hero_cross_put_bytes(HeroCross *b, const void *p, size_t n) {
    hero_cross_room(b, n);
    if (n > 0) memcpy(b->bytes + b->len, p, n);
    b->len += n;
}

/* A block this runtime wrote and only this runtime reads: reading past its end
 * is a runtime bug, told as one, never a read of what lies beyond. */
static void hero_cross_take_bytes(HeroCross *b, void *p, size_t n) {
    if (n > b->len - b->at) hero_panic("a value crossing between threads was read past its end — a runtime bug");
    if (n > 0) memcpy(p, b->bytes + b->at, n);
    b->at += n;
}

void hero_cross_free(HeroCross *b) {
    if (b->bytes != NULL) hero_free_raw(b->bytes);
    b->bytes = NULL;
    b->len = 0;
    b->cap = 0;
    b->at = 0;
}

/* A type that owns nothing, a number, a `bool`, a function, a record of them:
 * its bytes. */
void hero_cross_put_raw(HeroCross *b, const void *p, int64_t size) {
    if (size < 0) hero_panic("a value of a negative size crossed between threads — a compiler bug");
    hero_cross_put_bytes(b, p, (size_t)size);
}

void hero_cross_take_raw(HeroCross *b, void *p, int64_t size) {
    if (size < 0) hero_panic("a value of a negative size crossed between threads — a compiler bug");
    hero_cross_take_bytes(b, p, (size_t)size);
}

/* A `str`: its length, then its bytes, rebuilt as a block born on the taking
 * thread. */
void hero_cross_put_str(HeroCross *b, const void *p) {
    HeroStr s = *(const HeroStr *)p;
    hero_str_require(s);
    int64_t len = s.len;
    hero_cross_put_bytes(b, &len, sizeof len);
    hero_cross_put_bytes(b, s.ptr, (size_t)len);
}

void hero_cross_take_str(HeroCross *b, void *p) {
    int64_t len = 0;
    hero_cross_take_bytes(b, &len, sizeof len);
    if (len < 0 || (uint64_t)len > b->len - b->at) {
        hero_panic("a `str` crossing between threads was read past its end — a runtime bug");
    }
    *(HeroStr *)p = hero_str_from_bytes((const char *)(b->bytes + b->at), len);
    b->at += (size_t)len;
}

void hero_cross_put_failure(HeroCross *b, const void *p) {
    const HeroFailure *f = (const HeroFailure *)p;
    hero_cross_put_str(b, &f->code);
    hero_cross_put_str(b, &f->msg);
}

void hero_cross_take_failure(HeroCross *b, void *p) {
    HeroFailure *f = (HeroFailure *)p;
    hero_cross_take_str(b, &f->code);
    hero_cross_take_str(b, &f->msg);
}

/* `[T]`: its count, then each element by the element's own put; rebuilt as a
 * block of the taking thread's, each element taken into its place. */
void hero_cross_put_array(HeroCross *b, const void *p, HeroPutFn each) {
    const HeroArrayHeader *a = *(HeroArrayHeader *const *)p;
    hero_array_require(a);
    int64_t len = a->len;
    hero_cross_put_bytes(b, &len, sizeof len);
    const unsigned char *from = hero_array_data_const(a);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < len; i += 1) each(b, from + (size_t)i * size);
}

void hero_cross_take_array(HeroCross *b, void *p, const HeroDesc *elem, HeroTakeFn each) {
    int64_t len = 0;
    hero_cross_take_bytes(b, &len, sizeof len);
    if (len < 0) hero_panic("an array crossing between threads was read with a negative count — a runtime bug");
    HeroArrayHeader *out = hero_array_new(elem, len > 0 ? len : 1);
    unsigned char *to = hero_array_data(out);
    for (int64_t i = 0; i < len; i += 1) {
        each(b, to + (size_t)i * elem->size);
        out->len = i + 1;
    }
    *(HeroArrayHeader **)p = out;
}

/* `{K: V}`: its count, then each entry, key and value, in the order the
 * entries were first written, so the rebuilt map iterates as the sent one. */
void hero_cross_put_map(HeroCross *b, const void *p, HeroPutFn key, HeroPutFn value) {
    const HeroMapHeader *m = *(HeroMapHeader *const *)p;
    hero_map_require(m);
    int64_t len = m->len;
    hero_cross_put_bytes(b, &len, sizeof len);
    for (int64_t at = 0; at < len; at += 1) {
        int64_t bucket = hero_map_entry_bucket(m, at);
        key(b, hero_map_key_at_const(m, bucket));
        value(b, hero_map_val_at_const(m, bucket));
    }
}

void hero_cross_take_map(HeroCross *b, void *p, const HeroDesc *kd, const HeroDesc *vd, HeroTakeFn key,
                         HeroTakeFn value) {
    int64_t len = 0;
    hero_cross_take_bytes(b, &len, sizeof len);
    if (len < 0) hero_panic("a map crossing between threads was read with a negative count — a runtime bug");
    HeroMapHeader *out = hero_map_new(kd, vd, len);
    unsigned char *k = (unsigned char *)hero_malloc_raw(kd->size);
    unsigned char *v = (unsigned char *)hero_malloc_raw(vd->size);
    for (int64_t at = 0; at < len; at += 1) {
        key(b, k);
        value(b, v);
        /* `put` takes references of its own, so the taken pair is dropped. */
        hero_map_put(out, k, v);
        kd->drop(k);
        vd->drop(v);
    }
    hero_free_raw(k);
    hero_free_raw(v);
    *(HeroMapHeader **)p = out;
}

/* -- the queue --------------------------------------------------------------- */

#if defined(_WIN32)
typedef SRWLOCK hero_mail_mutex;
typedef CONDITION_VARIABLE hero_mail_cond;
#define HERO_MAIL_LIST_INIT SRWLOCK_INIT
#define hero_mail_take(m) AcquireSRWLockExclusive(m)
#define hero_mail_drop(m) ReleaseSRWLockExclusive(m)
#define hero_mail_wake_one(c) WakeConditionVariable(c)
#define hero_mail_wake_all(c) WakeAllConditionVariable(c)
#else
typedef pthread_mutex_t hero_mail_mutex;
typedef pthread_cond_t hero_mail_cond;
#define HERO_MAIL_LIST_INIT PTHREAD_MUTEX_INITIALIZER
#define hero_mail_take(m) pthread_mutex_lock(m)
#define hero_mail_drop(m) pthread_mutex_unlock(m)
#define hero_mail_wake_one(c) pthread_cond_signal(c)
#define hero_mail_wake_all(c) pthread_cond_broadcast(c)
#endif

/* One value waiting in a queue, in its flat encoding. */
typedef struct HeroMailNote {
    struct HeroMailNote *next;
    HeroCross value;
} HeroMailNote;

struct HeroMail {
    /* How many of the program's values reach this queue. */
    _Atomic int64_t copies;
    /* Every field below, but the live list's two, is under this lock. */
    hero_mail_mutex lock;
    hero_mail_cond ready;
    HeroMailNote *head;
    HeroMailNote *tail;
    int64_t queued;
    int closed;
    /* 1 once a receive found it empty after the stop: `stopped` for ever. */
    int stopped;
    /* How a queued value is rebuilt, and given back, when its queue goes
     * first. */
    HeroTakeFn take;
    const HeroDesc *desc;
    /* Where `mailbox()` stood, for the end of `main` to name. */
    const HeroPlace *begun;
    /* The live list the stop and the end of `main` walk, under its own lock. */
    struct HeroMail *live_prev;
    struct HeroMail *live_next;
    /* Its number's slot plus one, 0 until `m.number()` first asks; written
     * under the list's lock, as the slots' table is. */
    int64_t slot;
    /* The platform layer's watches (`hero_layer_mail.h`), each rung under
     * this queue's lock, `watch_room` of them allocated. */
    struct HeroMailWatch *watches;
    int64_t watching;
    int64_t watch_room;
};

/* One watch: the layer's ring and its context. */
typedef struct HeroMailWatch {
    HeroMailRing ring;
    void *ctx;
} HeroMailWatch;

/* A queue known by number (`m.number()`): the queue, NULL while the slot is
 * free; the life the slot is in, which the number carries, so a number whose
 * queue has gone names nothing and never the next queue of that slot; and the
 * next free slot plus one. */
typedef struct {
    HeroMail *queue;
    uint32_t life;
    int64_t next_free;
} HeroMailSlot;

/* SHARED ON PURPOSE, for `tests/harness/suite_runtime.hero`'s rule 3: the
 * queues alive, so a stop can wake a receive waiting on any of them and the
 * end of `main` can count them, under `hero_mail_list_lock`; and the stop's
 * flag, atomic. */
static hero_mail_mutex hero_mail_list_lock = HERO_MAIL_LIST_INIT;
static HeroMail *hero_mail_list = NULL;
static int64_t hero_mail_alive = 0;
static _Atomic int hero_mail_stop_asked = 0;

/* SHARED ON PURPOSE, as the list is and under its lock: the slots a number
 * names (`hero_mail_number`), `hero_mail_slot_room` of them allocated, the
 * first `hero_mail_slots_used` ever handed out, and the first free one plus
 * one, 0 for none. A number is read on any thread, so a per-thread table would
 * name nothing for the thread that waits. */
static HeroMailSlot *hero_mail_slots = NULL;
static int64_t hero_mail_slot_room = 0;
static int64_t hero_mail_slots_used = 0;
static int64_t hero_mail_slot_free = 0;

/* The queue whose values this thread is giving back, while it gives them back:
 * no thread's and no C handle's holder moves then; and, where the queue's last
 * copy went with values still in it rather than a send failing, each C handle
 * rebuilt is marked as dropped with that mailbox (`hero_mail_marking`). */
static _Thread_local HeroMail *hero_mail_dropping = NULL;
static _Thread_local int hero_mail_marking = 0;

HeroMail *hero_mail_new(HeroTakeFn take, const HeroDesc *desc, const HeroPlace *where) {
    if (take == NULL || desc == NULL) hero_mail_panic("a mailbox was begun with no way to rebuild its values — a compiler bug", where);
    HeroMail *m = (HeroMail *)hero_calloc_raw(sizeof *m);
    atomic_init(&m->copies, 1);
#if defined(_WIN32)
    InitializeSRWLock(&m->lock);
    InitializeConditionVariable(&m->ready);
#else
    pthread_mutex_init(&m->lock, NULL);
#if defined(__APPLE__)
    pthread_cond_init(&m->ready, NULL);
#else
    /* Waited on against the monotonic clock, so a clock set back does not
     * stretch a receive's bound. */
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    pthread_cond_init(&m->ready, &attr);
    pthread_condattr_destroy(&attr);
#endif
#endif
    m->take = take;
    m->desc = desc;
    m->begun = where;
    hero_mail_take(&hero_mail_list_lock);
    m->live_next = hero_mail_list;
    if (hero_mail_list != NULL) hero_mail_list->live_prev = m;
    hero_mail_list = m;
    hero_mail_alive += 1;
    hero_mail_drop(&hero_mail_list_lock);
    return m;
}

/* A queued value whose queue goes, or whose send failed: rebuilt here and
 * dropped, so what it holds (a mailbox's count, a counted block) is given back
 * on this thread, and a C handle in it is marked as dropped with the mailbox,
 * which the end of `main` names as one nobody gave back. */
static void hero_mail_drop_note(HeroMail *m, HeroMailNote *n, int with_queue) {
    HeroMail *outer = hero_mail_dropping;
    int outer_marking = hero_mail_marking;
    hero_mail_dropping = m;
    hero_mail_marking = with_queue;
    /* A value of 256 bytes or fewer is rebuilt on the stack. Not named
     * `small`: the Windows SDK's `rpcndr.h`, which `windows.h` includes,
     * defines `small` as a macro for `char`. */
    _Alignas(max_align_t) unsigned char on_stack[256];
    void *v = m->desc->size <= sizeof on_stack ? (void *)on_stack : hero_malloc_raw(m->desc->size);
    n->value.at = 0;
    m->take(&n->value, v);
    m->desc->drop(v);
    if (v != (void *)on_stack) hero_free_raw(v);
    hero_cross_free(&n->value);
    hero_free_raw(n);
    hero_mail_dropping = outer;
    hero_mail_marking = outer_marking;
}

void hero_mail_incref(HeroMail *m) {
    if (m == NULL) return;
    atomic_fetch_add_explicit(&m->copies, 1, memory_order_relaxed);
}

void hero_mail_decref(HeroMail *m) {
    if (m == NULL) return;
    if (atomic_fetch_sub_explicit(&m->copies, 1, memory_order_acq_rel) != 1) return;
    /* The last copy: no other thread reaches this queue now, but the stop may
     * be walking the list, so it leaves the list first, under the list's lock. */
    hero_mail_take(&hero_mail_list_lock);
    if (m->live_prev != NULL) {
        m->live_prev->live_next = m->live_next;
    } else {
        hero_mail_list = m->live_next;
    }
    if (m->live_next != NULL) m->live_next->live_prev = m->live_prev;
    hero_mail_alive -= 1;
    /* Its number names nothing from now: the slot is free, its life kept, so
     * the next queue there is numbered with the next life. Every watch is
     * rung once, so a wait on the number wakes and finds it gone; a watch, an
     * unwatch and a readiness asked by number all take the list's lock first,
     * so none reaches this queue meanwhile. */
    if (m->slot > 0) {
        HeroMailSlot *at = &hero_mail_slots[m->slot - 1];
        at->queue = NULL;
        if (at->life < UINT32_MAX) {
            at->next_free = hero_mail_slot_free;
            hero_mail_slot_free = m->slot;
        }
    }
    for (int64_t i = 0; i < m->watching; i++) m->watches[i].ring(m->watches[i].ctx);
    hero_mail_drop(&hero_mail_list_lock);
    if (m->watches != NULL) hero_free_raw(m->watches);
    HeroMailNote *n = m->head;
    m->head = NULL;
    m->tail = NULL;
    while (n != NULL) {
        HeroMailNote *next = n->next;
        /* May give back another queue's copy, never this one's. */
        hero_mail_drop_note(m, n, 1);
        n = next;
    }
#if !defined(_WIN32)
    pthread_cond_destroy(&m->ready);
    pthread_mutex_destroy(&m->lock);
#endif
    hero_free_raw(m);
}

/* The exit sweep's release, through the slot's address. */
void hero_mail_release_at(HeroMail **slot) {
    HeroMail *m = *slot;
    *slot = NULL;
    hero_mail_decref(m);
}

/* A `(mailbox U)` inside a value: its address, the copy's count travelling with
 * it, so the queue stands while the value is in transit. */
void hero_cross_put_mail(HeroCross *b, const void *p) {
    HeroMail *m = *(HeroMail *const *)p;
    if (m == NULL) hero_panic("a mailbox no `mailbox()` began crossed between threads — a compiler bug");
    hero_mail_incref(m);
    hero_cross_put_bytes(b, &m, sizeof m);
}

void hero_cross_take_mail(HeroCross *b, void *p) { hero_cross_take_bytes(b, p, sizeof(HeroMail *)); }

#define HERO_MAIL_GOT 0
#define HERO_MAIL_TIMEOUT 1
#define HERO_MAIL_CLOSED 2
#define HERO_MAIL_STOPPED 3

static HeroMail *hero_mail_named(HeroMail *m, const char *what, const HeroPlace *where) {
    if (m == NULL) hero_mail_panic(what, where);
    return m;
}

/* Every watch of the queue rung, under its lock, which an unwatch takes too:
 * so a watch taken away is never rung after. A ring takes no lock and does
 * not wait (`hero_layer_mail.h`). */
static void hero_mail_ring(HeroMail *m) {
    for (int64_t i = 0; i < m->watching; i++) m->watches[i].ring(m->watches[i].ctx);
}

/* `m.send(v)`: the copy written on this thread, outside the lock; then, under
 * it, `closed` or `stopped` with the copy given back and `*v` untouched, or
 * every handle `*v` holds handed over and its cell emptied (`sent`, NULL for a
 * type that holds none) before the copy is queued, so no receiver can take a
 * handle over before it was handed over. */
int64_t hero_mail_send(HeroMail *m, HeroPutFn put, HeroSentFn sent, void *v, const HeroPlace *where) {
    hero_mail_named(m, "a value was sent to a mailbox no `mailbox()` began — a compiler bug", where);
    if (put == NULL) hero_mail_panic("a value was sent with no way to write it — a compiler bug", where);
    HeroMailNote *n = (HeroMailNote *)hero_calloc_raw(sizeof *n);
    put(&n->value, v);
    hero_mail_take(&m->lock);
    int64_t refused = m->closed ? HERO_MAIL_CLOSED : m->stopped ? HERO_MAIL_STOPPED : HERO_MAIL_GOT;
    if (refused != HERO_MAIL_GOT) {
        hero_mail_drop(&m->lock);
        hero_mail_drop_note(m, n, 0);
        return refused;
    }
    if (sent != NULL) sent(v);
    if (m->tail != NULL) {
        m->tail->next = n;
    } else {
        m->head = n;
    }
    m->tail = n;
    m->queued += 1;
    hero_mail_wake_one(&m->ready);
    /* From empty to one value is the one change a watch is rung for: while
     * the queue holds a value it is ready, and the layer asks before waiting. */
    if (m->queued == 1) hero_mail_ring(m);
    hero_mail_drop(&m->lock);
    return HERO_MAIL_GOT;
}

/* Milliseconds on a clock that never runs back. */
static int64_t hero_mail_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000 + (int64_t)(now.tv_nsec / 1000000);
#endif
}

/* One wait on the queue's condition, at most `ms` milliseconds, or with no
 * limit where `ms` is -1. Its lock held. */
static void hero_mail_wait(HeroMail *m, int64_t ms) {
#if defined(_WIN32)
    DWORD slice = ms < 0 ? INFINITE : ms > 0x7ffffffe ? 0x7ffffffe : (DWORD)ms;
    SleepConditionVariableSRW(&m->ready, &m->lock, slice, 0);
#else
    if (ms < 0) {
        pthread_cond_wait(&m->ready, &m->lock);
        return;
    }
#if defined(__APPLE__)
    struct timespec rel;
    rel.tv_sec = (time_t)(ms / 1000);
    rel.tv_nsec = (long)((ms % 1000) * 1000000);
    pthread_cond_timedwait_relative_np(&m->ready, &m->lock, &rel);
#else
    struct timespec at;
    clock_gettime(CLOCK_MONOTONIC, &at);
    at.tv_sec += (time_t)(ms / 1000);
    at.tv_nsec += (long)((ms % 1000) * 1000000);
    if (at.tv_nsec >= 1000000000L) {
        at.tv_sec += 1;
        at.tv_nsec -= 1000000000L;
    }
    pthread_cond_timedwait(&m->ready, &m->lock, &at);
#endif
#endif
}

/* `m.receive(within_ms: n)`: 0 with the oldest value rebuilt into `*out` on
 * this thread, every handle in it taken over; 1 `timeout`; 2 `closed`, closed
 * and empty; 3 `stopped`, empty once the stop was asked, and from then on. */
int64_t hero_mail_receive(HeroMail *m, void *out, int64_t within_ms, const HeroPlace *where) {
    hero_mail_named(m, "a mailbox no `mailbox()` began was received from — a compiler bug", where);
    if (within_ms < -1) {
        hero_mail_panic("a mailbox's receive was given a time below -1 — `within_ms` is how many milliseconds "
                        "it may wait, 0 for none and -1 for no limit",
                        where);
    }
    int64_t until = 0;
    if (within_ms >= 0) {
        int64_t now = hero_mail_now_ms();
        until = within_ms > INT64_MAX - now ? INT64_MAX : now + within_ms;
    }
    int64_t answer = HERO_MAIL_TIMEOUT;
    HeroMailNote *got = NULL;
    hero_mail_take(&m->lock);
    for (;;) {
        if (m->head != NULL) {
            got = m->head;
            m->head = got->next;
            if (m->head == NULL) m->tail = NULL;
            m->queued -= 1;
            answer = HERO_MAIL_GOT;
            break;
        }
        if (m->closed) {
            answer = HERO_MAIL_CLOSED;
            break;
        }
        if (m->stopped || atomic_load(&hero_mail_stop_asked) != 0) {
            m->stopped = 1;
            answer = HERO_MAIL_STOPPED;
            break;
        }
        int64_t left = -1;
        if (within_ms >= 0) {
            left = until - hero_mail_now_ms();
            if (left <= 0) {
                answer = HERO_MAIL_TIMEOUT;
                break;
            }
        }
        hero_mail_wait(m, left);
    }
    hero_mail_drop(&m->lock);
    if (got != NULL) {
        got->value.at = 0;
        m->take(&got->value, out);
        hero_cross_free(&got->value);
        hero_free_raw(got);
    }
    return answer;
}

/* `m.close()`: for every copy; closing again does nothing. A receive waiting
 * on an empty queue wakes and answers `closed`. */
void hero_mail_close(HeroMail *m, const HeroPlace *where) {
    hero_mail_named(m, "a mailbox no `mailbox()` began was closed — a compiler bug", where);
    hero_mail_take(&m->lock);
    int was = m->closed;
    m->closed = 1;
    hero_mail_wake_all(&m->ready);
    if (!was) hero_mail_ring(m);
    hero_mail_drop(&m->lock);
}

/* The stop, heard: what `hero_net.c`'s `hero_net_nudge_all` calls each time it
 * hears one. Every waiting receive looks again. The list's lock, then each
 * queue's: the one order. */
void hero_mail_hear_stop(void) {
    atomic_store(&hero_mail_stop_asked, 1);
    hero_mail_take(&hero_mail_list_lock);
    for (HeroMail *m = hero_mail_list; m != NULL; m = m->live_next) {
        hero_mail_take(&m->lock);
        hero_mail_wake_all(&m->ready);
        hero_mail_ring(m);
        hero_mail_drop(&m->lock);
    }
    hero_mail_drop(&hero_mail_list_lock);
}

/* -- a queue named by number (panel 256's R6 to R8, provisional) -------------- */

/* The number's halves: the slot plus one above 32 bits, the slot's life
 * below, so a number is above 0 and fits an `i64` with room to spare. */
#define HERO_MAIL_SLOT_MOST INT64_C(0x7fffffff)

/* `m.number()`: the same for every copy, the slot taken the first time it is
 * asked, held by no copy. */
int64_t hero_mail_number(HeroMail *m, const HeroPlace *where) {
    hero_mail_named(m, "a mailbox no `mailbox()` began was asked its number — a compiler bug", where);
    hero_mail_take(&hero_mail_list_lock);
    if (m->slot == 0) {
        int64_t at = 0;
        if (hero_mail_slot_free != 0) {
            at = hero_mail_slot_free;
            hero_mail_slot_free = hero_mail_slots[at - 1].next_free;
        } else {
            if (hero_mail_slots_used == HERO_MAIL_SLOT_MOST) {
                hero_mail_drop(&hero_mail_list_lock);
                hero_mail_panic("more than 2,147,483,647 mailboxes were numbered at once", where);
            }
            if (hero_mail_slots_used == hero_mail_slot_room) {
                int64_t room = hero_mail_slot_room == 0 ? 16 : hero_mail_slot_room * 2;
                if (room > HERO_MAIL_SLOT_MOST) room = HERO_MAIL_SLOT_MOST;
                HeroMailSlot *grown = (HeroMailSlot *)hero_calloc_raw((size_t)room * sizeof *grown);
                if (hero_mail_slot_room > 0) {
                    memcpy(grown, hero_mail_slots, (size_t)hero_mail_slot_room * sizeof *grown);
                    hero_free_raw(hero_mail_slots);
                }
                hero_mail_slots = grown;
                hero_mail_slot_room = room;
            }
            hero_mail_slots_used += 1;
            at = hero_mail_slots_used;
        }
        HeroMailSlot *s = &hero_mail_slots[at - 1];
        s->queue = m;
        s->life += 1;
        s->next_free = 0;
        m->slot = at;
    }
    int64_t number = (m->slot << 32) | (int64_t)hero_mail_slots[m->slot - 1].life;
    hero_mail_drop(&hero_mail_list_lock);
    return number;
}

/* Under the list's lock: the queue a number names, NULL once it has gone. */
static HeroMail *hero_mail_numbered(int64_t number) {
    if (number <= 0) return NULL;
    int64_t slot = number >> 32;
    if (slot < 1 || slot > hero_mail_slots_used) return NULL;
    const HeroMailSlot *s = &hero_mail_slots[slot - 1];
    if (s->queue == NULL || (int64_t)s->life != (number & INT64_C(0xffffffff))) return NULL;
    return s->queue;
}

int64_t hero_mail_ready(int64_t number) {
    int64_t ready = -1;
    hero_mail_take(&hero_mail_list_lock);
    HeroMail *m = hero_mail_numbered(number);
    if (m != NULL) {
        hero_mail_take(&m->lock);
        ready = m->head != NULL || m->closed || m->stopped || atomic_load(&hero_mail_stop_asked) != 0 ? 1 : 0;
        hero_mail_drop(&m->lock);
    }
    hero_mail_drop(&hero_mail_list_lock);
    return ready;
}

int64_t hero_mail_watch(int64_t number, HeroMailRing ring, void *ctx) {
    if (ring == NULL) return -1;
    int64_t answer = -1;
    hero_mail_take(&hero_mail_list_lock);
    HeroMail *m = hero_mail_numbered(number);
    if (m != NULL) {
        hero_mail_take(&m->lock);
        if (m->watching == m->watch_room) {
            int64_t room = m->watch_room == 0 ? 4 : m->watch_room * 2;
            HeroMailWatch *grown = (HeroMailWatch *)hero_calloc_raw((size_t)room * sizeof *grown);
            if (m->watching > 0) memcpy(grown, m->watches, (size_t)m->watching * sizeof *grown);
            if (m->watches != NULL) hero_free_raw(m->watches);
            m->watches = grown;
            m->watch_room = room;
        }
        m->watches[m->watching].ring = ring;
        m->watches[m->watching].ctx = ctx;
        m->watching += 1;
        answer = 0;
        hero_mail_drop(&m->lock);
    }
    hero_mail_drop(&hero_mail_list_lock);
    return answer;
}

int64_t hero_mail_unwatch(int64_t number, HeroMailRing ring, void *ctx) {
    int64_t answer = -1;
    hero_mail_take(&hero_mail_list_lock);
    HeroMail *m = hero_mail_numbered(number);
    if (m != NULL) {
        hero_mail_take(&m->lock);
        for (int64_t i = 0; i < m->watching; i++) {
            if (m->watches[i].ring == ring && m->watches[i].ctx == ctx) {
                m->watches[i] = m->watches[m->watching - 1];
                m->watching -= 1;
                answer = 0;
                break;
            }
        }
        hero_mail_drop(&m->lock);
    }
    hero_mail_drop(&hero_mail_list_lock);
    return answer;
}

/* -- the failures a receive, a send and a start answer ---------------------- */

HeroFailure hero_failure_timeout(void) {
    HERO_STR_STATIC(code, "timeout");
    HERO_STR_STATIC(msg, "no value arrived in the mailbox within the time given; one that arrives later is the "
                         "next receive's");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_stopped(void) {
    HERO_STR_STATIC(code, "stopped");
    HERO_STR_STATIC(msg, "the program's stop was asked and the mailbox was found empty, so it answers `stopped` "
                         "from then on");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_closed(void) {
    HERO_STR_STATIC(code, "closed");
    HERO_STR_STATIC(msg, "the mailbox was closed: it takes no more values, and every value sent before the "
                         "close has been received");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

/* A send's refusal says what a send refused, not what a receive found. */
HeroFailure hero_failure_send_closed(void) {
    HERO_STR_STATIC(code, "closed");
    HERO_STR_STATIC(msg, "the mailbox was closed, so it takes no more values; the value sent is left as it was");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_send_stopped(void) {
    HERO_STR_STATIC(code, "stopped");
    HERO_STR_STATIC(msg, "the mailbox answered `stopped` once the program's stop was asked, so it takes no more "
                         "values; the value sent is left as it was");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

/* `spawn`'s refusal, its message saying why: the runtime's table of 256 full,
 * or the system's own number. Built as the program's own failure, its two
 * texts counted blocks, since the message is made at run time. */
HeroFailure hero_failure_limit(int64_t why) {
    char text[200];
    if (why == HERO_SPAWN_SLOTS) {
        snprintf(text, sizeof text,
                 "no thread could be started: 256 of this program's threads are running, the most this "
                 "runtime holds, and a thread's room comes back when it is waited for");
    } else {
        snprintf(text, sizeof text, "no thread could be started: the system refused one (error %lld)",
                 (long long)why);
    }
    return (HeroFailure){hero_str_from_bytes("limit", 5), hero_str_from_bytes(text, (int64_t)strlen(text))};
}

/* -- the language's threads ---------------------------------------------------
 *
 * The program's C fills a box (`HeroThreadBox` and what follows it: the
 * function), writes the argument into the box's `arg`, starts it here, and at
 * the wait rebuilds the answer from the box's `answer`, which the new thread
 * wrote. The box is the runtime's own memory, weighed by no counter, as
 * `parts/spawn_str.c`'s is; the values crossing in it are flat. A thread's
 * value is its slot and the generation of its start, never an address; the
 * slot records the thread that holds it (R3). */

#define HERO_THREAD_SLOT_BITS 16
/* What `m.send(@t)` leaves in `t`'s cell: no start ever had it. */
#define HERO_THREAD_SENT ((int64_t)-2)

static int64_t hero_thread_lang_run(int64_t number) {
    HeroThreadBox *box = (HeroThreadBox *)(uintptr_t)number;
    box->run(box);
    return number;
}

/* The slot a thread's value names, its generation checked, or -1. Under the
 * table's lock. */
static int64_t hero_thread_slot_of(int64_t handle) {
    if (handle <= 0) return -1;
    int64_t at = (int64_t)((uint64_t)handle & (((uint64_t)1 << HERO_THREAD_SLOT_BITS) - 1));
    uint64_t generation = (uint64_t)handle >> HERO_THREAD_SLOT_BITS;
    if (at >= HERO_SPAWN_SLOTS) return -1;
    if (hero_spawn_slots[at].state == 0 || hero_spawn_slots[at].kind != HERO_SPAWN_LANG ||
        hero_spawn_slots[at].generation != generation) {
        return -1;
    }
    return at;
}

int64_t hero_thread_begin(HeroThreadBox *box, int64_t bytes, int64_t *why, const HeroPlace *where) {
    if (box == NULL || box->run == NULL) hero_mail_panic("a thread was started with no function to run", where);
    if (bytes < 0) {
        hero_mail_panic("a thread was asked for a stack of fewer than 0 bytes — `bytes` is the least size of its "
                        "stack",
                        where);
    }
    int64_t refused = 0;
    int64_t at = hero_spawn_start(hero_thread_lang_run, (int64_t)(uintptr_t)box, (size_t)bytes, HERO_SPAWN_LANG,
                                  &refused, (uintptr_t)box->body, (uintptr_t)__builtin_return_address(0));
    if (at < 0) {
        if (why != NULL) *why = refused;
        return -1;
    }
    uint64_t me = hero_this_thread();
    HERO_SPAWN_TAKE();
    hero_spawn_slots[at].place = where;
    hero_spawn_slots[at].holder = me;
    uint64_t generation = hero_spawn_slots[at].generation;
    HERO_SPAWN_DROP();
    return (int64_t)((generation << HERO_THREAD_SLOT_BITS) | (uint64_t)at);
}

HeroThreadBox *hero_thread_end(int64_t handle, const HeroPlace *where) {
    if (handle == HERO_THREAD_SENT) {
        hero_mail_panic("a thread was waited for through a value `m.send(@t)` emptied: the thread was handed to "
                        "whichever thread receives it from that mailbox, and that thread waits for it",
                        where);
    }
    uint64_t me = hero_this_thread();
    HERO_SPAWN_TAKE();
    int64_t at = hero_thread_slot_of(handle);
    uint64_t holder = at >= 0 ? hero_spawn_slots[at].holder : 0;
    int fresh = at >= 0 && holder == me;
    /* Claimed under the lock, so two waits through two copies never both join. */
    if (fresh) hero_spawn_slots[at].holder = 0;
    HERO_SPAWN_DROP();
    if (at < 0) {
        hero_mail_panic("a thread was waited for twice, or one this program never started — `t.wait()` ends the "
                        "thread's life, and every copy of it with it",
                        where);
    }
    if (!fresh) {
        hero_mail_panic(holder == HERO_HELD_IN_TRANSIT
                            ? "a thread was waited for while it is inside a mailbox: `m.send(@t)` handed it to "
                              "whichever thread receives it, and this is a copy kept from before the send"
                        : holder == 0
                            ? "a thread was waited for twice through two copies of it — `t.wait()` ends the "
                              "thread's life, and every copy of it with it"
                            : "a thread was waited for on a thread that does not hold it: a thread belongs to the "
                              "thread that started it or last received it, from a mailbox or as a thread's answer, "
                              "and only that one waits for it",
                        where);
    }
    return (HeroThreadBox *)(uintptr_t)hero_spawn_join(at, HERO_SPAWN_LANG, where);
}

/* A thread inside a value crossing between threads: its number; the sender's
 * `sent` hands it over, and the receiver's take takes it over, unless the
 * value is being given back with its queue. */
void hero_cross_put_thread(HeroCross *b, const void *p) { hero_cross_put_bytes(b, p, sizeof(int64_t)); }

void hero_cross_take_thread(HeroCross *b, void *p) {
    hero_cross_take_bytes(b, p, sizeof(int64_t));
    if (hero_mail_dropping != NULL) return;
    int64_t handle = *(int64_t *)p;
    uint64_t me = hero_this_thread();
    HERO_SPAWN_TAKE();
    int64_t at = hero_thread_slot_of(handle);
    if (at >= 0 && hero_spawn_slots[at].holder == HERO_HELD_IN_TRANSIT) hero_spawn_slots[at].holder = me;
    HERO_SPAWN_DROP();
}

/* `m.send(@t)` succeeded, or a thread's answer leaves it: `t` is handed over
 * and its cell emptied. */
void hero_thread_sent(void *cell) {
    int64_t handle = *(int64_t *)cell;
    HERO_SPAWN_TAKE();
    int64_t at = hero_thread_slot_of(handle);
    if (at >= 0) hero_spawn_slots[at].holder = HERO_HELD_IN_TRANSIT;
    HERO_SPAWN_DROP();
    *(int64_t *)cell = HERO_THREAD_SENT;
}

/* A C handle inside a value crossing between threads: its address; its life,
 * in the process-wide handle set (`parts/alloc.c`), moves by the hand-over. */
void hero_cross_put_handle(HeroCross *b, const void *p) { hero_cross_put_bytes(b, p, sizeof(void *)); }

void hero_cross_take_handle(HeroCross *b, void *p) {
    hero_cross_take_bytes(b, p, sizeof(void *));
    const void *h = *(void *const *)p;
    if (hero_mail_dropping != NULL) {
        if (hero_mail_marking) hero_handle_dropped_with_mail(h, hero_mail_dropping->begun);
        return;
    }
    hero_handle_take_over(h);
}

/* `m.send(@v)` succeeded, or a thread's answer leaves it: the handle in `*cell`
 * is handed over and the cell holds the value a send leaves (`parts/alloc.c`). */
void hero_handle_sent(void *cell) {
    const void *h = *(void *const *)cell;
    if (h == NULL) return;
    hero_handle_hand_over(h);
    *(void **)cell = hero_handle_sent_value();
}

/* An array or a map that holds handles, sent: each element's handed over by
 * `each`, on a copy the sender's cell no longer reaches, and the cell left
 * empty, so the sender's other copies keep their own values. */
void hero_cross_sent_array(void *cell, HeroSentFn each) {
    HeroArrayHeader *a = *(HeroArrayHeader **)cell;
    hero_array_require(a);
    unsigned char *at = hero_array_data(a);
    size_t size = a->elem->size;
    _Alignas(max_align_t) unsigned char one[256];
    void *slot = size <= sizeof one ? (void *)one : hero_malloc_raw(size);
    for (int64_t i = 0; i < a->len; i += 1) {
        memcpy(slot, at + (size_t)i * size, size);
        each(slot);
    }
    if (slot != (void *)one) hero_free_raw(slot);
    HeroArrayHeader *empty = hero_array_new(a->elem, 1);
    hero_array_decref(a);
    *(HeroArrayHeader **)cell = empty;
}

void hero_cross_sent_map(void *cell, HeroSentFn key, HeroSentFn value) {
    HeroMapHeader *m = *(HeroMapHeader **)cell;
    hero_map_require(m);
    unsigned char *k = (unsigned char *)hero_malloc_raw(m->key->size);
    unsigned char *v = (unsigned char *)hero_malloc_raw(m->val->size);
    for (int64_t at = 0; at < m->len; at += 1) {
        int64_t bucket = hero_map_entry_bucket(m, at);
        if (key != NULL) {
            memcpy(k, hero_map_key_at_const(m, bucket), m->key->size);
            key(k);
        }
        if (value != NULL) {
            memcpy(v, hero_map_val_at_const(m, bucket), m->val->size);
            value(v);
        }
    }
    hero_free_raw(k);
    hero_free_raw(v);
    HeroMapHeader *empty = hero_map_new(m->key, m->val, 0);
    hero_map_decref(m);
    *(HeroMapHeader **)cell = empty;
}

void hero_thread_box_free(HeroThreadBox *box) {
    hero_cross_free(&box->arg);
    hero_cross_free(&box->answer);
    hero_free_raw(box);
}

void *hero_thread_box_new(int64_t size) {
    if (size < (int64_t)sizeof(HeroThreadBox)) hero_panic("a thread's box smaller than its head — a compiler bug");
    return hero_calloc_raw((size_t)size);
}

/* -- the end of `main` --------------------------------------------------------
 *
 * After the threads (`hero_spawn_check_joined`), before any balance: a queue
 * still alive is held by a copy of itself inside a queue, its own or one it
 * holds in turn, which the checker's rule refuses (R5); told with how many and
 * where the first eight were begun. */
#define HERO_MAIL_NAMED 8

static void hero_mail_check_alive(void) {
    hero_mail_take(&hero_mail_list_lock);
    int64_t alive = hero_mail_alive;
    int64_t named = 0;
    const HeroPlace *begun[HERO_MAIL_NAMED];
    for (HeroMail *m = hero_mail_list; m != NULL && named < HERO_MAIL_NAMED; m = m->live_next) {
        begun[named++] = m->begun;
    }
    hero_mail_drop(&hero_mail_list_lock);
    if (alive == 0) return;
    hero_report_begin();
    hero_report_printf("panic: %lld mailbox(es) still alive when `main` returned and every thread was waited "
                       "for: only a copy of a mailbox inside a queue, its own or one it holds in turn, keeps one "
                       "alive past that, and no queue may hold itself\n",
                       (long long)alive);
    for (int64_t k = 0; k < named; k += 1) {
        if (begun[k] != NULL) {
            hero_report_printf("  a mailbox begun at %s:%lld:%lld, in %s\n", begun[k]->path,
                               (long long)begun[k]->line, (long long)begun[k]->column, begun[k]->name);
        }
    }
    hero_abort();
}

/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/mailbox.c — threads that exchange any value while they run: the
 * language's `spawn`, `t.wait()`, `mailbox()`, `m.send(v)`, `m.receive(...)`
 * and `m.close()` (spec § 3, § 11, § 13; feature 2647; design.md Part 7.13).
 *
 * EVERY VALUE CROSSES WHOLE, AS A COPY. The sending thread copies the value
 * into blocks no other thread holds, by the copier the compiler wrote for its
 * type (`HeroDeepFn`, `emit/crossing.hero`): a `str`'s bytes into a block of
 * their own, an array's elements and a map's entries one by one, a record's
 * fields, a case's payload. Copy-on-write's test and its write are two steps no
 * atomic makes one (`parts/cow.c`), so two threads holding one block could
 * both read themselves its only holder and both write; a copy leaves nothing
 * for two threads to hold. This runtime has one heap, the C library's, behind
 * `parts/alloc.c`'s single point: "the receiver's heap" of Part 7.13 is here
 * the blocks only the receiver holds, which the copy makes, and a block made
 * on the sending thread is given back on the receiving one, which the C
 * library's allocator allows and both counters of the exit gate weigh as one
 * balance (`parts/alloc.c`, BOTH COUNTERS ARE ATOMIC).
 *
 * A HANDLE IS A NUMBER, NEVER AN ADDRESS. A thread's or a mailbox's value is
 * its slot in a table of this file's and the generation of the life that slot
 * holds, packed in one `int64_t`: a copy kept past the life's end names a
 * generation that is over, so it is told by name rather than followed into a
 * slot another life holds now. Zero is no life at all, so a slot the program
 * never wrote is told too.
 *
 * A MAILBOX IS A QUEUE EVERY COPY REACHES (spec § 3's address sentence). Its
 * lock and its condition are the platform's (pthreads, and Windows' slim lock
 * and condition variable under `_WIN32`), made once when its slot is first
 * made and never given back, so a thread holding a stale copy always finds a
 * lock to take. `close` says no more values will come: what is queued is still
 * received, oldest first, and a receive past the last fails `closed`; the slot
 * goes back to the table once it is closed and empty. A send to a closed
 * mailbox, and a second close, abort: the life of the queue is over for every
 * copy. When `main` returns, a mailbox nobody closed, and a value sent and
 * never received, end the program as a lease nobody ended does (spec § 13).
 *
 * THE STOP. Once the program's stop is asked (`hero_net.h`'s, by a signal it
 * caught or by `hero_net_ask_stop`), a receive that would wait fails
 * `stopped`, and one already waiting wakes and fails `stopped`; a value
 * already queued is still received, as `hero_net.h` still moves bytes already
 * there. The platform layer tells this file of the stop through
 * `hero_layer_stop_heard` (`platform/hero_layer_stop.h`), on a thread of its
 * own and never inside a signal handler. */

#if !defined(_WIN32)
/* Named here rather than inherited, `parts/spawn.c`'s reason: a file that leans
 * on another file's includes works until that file grows a configuration. */
#include <pthread.h>
#include <time.h>
#endif

#if defined(_WIN32)
typedef SRWLOCK hero_box_mutex;
typedef CONDITION_VARIABLE hero_box_cond;
#define HERO_BOX_MUTEX_INIT SRWLOCK_INIT
#define hero_box_take(m) AcquireSRWLockExclusive(m)
#define hero_box_drop(m) ReleaseSRWLockExclusive(m)
#else
typedef pthread_mutex_t hero_box_mutex;
typedef pthread_cond_t hero_box_cond;
#define HERO_BOX_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define hero_box_take(m) pthread_mutex_lock(m)
#define hero_box_drop(m) pthread_mutex_unlock(m)
#endif

/* THE TABLE: up to 2^20 mailboxes open at once, in chunks of 1024 made as they
 * are first needed and kept for the life of the process, so a slot's lock and
 * condition stand wherever a stale copy may reach for them. The generation
 * takes the other 43 bits of the handle, so a slot serves 8.8e12 lives before
 * it is retired rather than reused. */
#define HERO_BOX_SLOT_BITS 20
#define HERO_BOX_CHUNK 1024
#define HERO_BOX_CHUNKS ((1 << HERO_BOX_SLOT_BITS) / HERO_BOX_CHUNK)
#define HERO_BOX_LAST_GENERATION (((uint64_t)1 << 43) - 1)

/* One value waiting in a queue: the next, then the value's bytes, aligned for
 * any C type the program's values are made of. */
typedef struct HeroBoxNode {
    struct HeroBoxNode *next;
    _Alignas(16) unsigned char value[];
} HeroBoxNode;

typedef struct {
    hero_box_mutex lock;
    hero_box_cond ready;
    /* The generation of the life this slot holds, or held last; 0 never. */
    uint64_t generation;
    /* 1 while the life stands open, 0 once closed. */
    int open;
    /* 1 while the slot is a life's, closed or not, until it is closed and
     * empty and goes back to the table. */
    int held;
    HeroBoxNode *head;
    HeroBoxNode *tail;
    int64_t queued;
    /* Threads inside a receive of this mailbox, waiting or about to. */
    int64_t waiting;
    /* The bytes of one value, the compiler's `sizeof`, asked of every send
     * and receive so a copy of another type's handle is told, not read. */
    size_t size;
    /* Where the life began and, once it is closed, where it was closed. */
    const HeroPlace *begun;
    const HeroPlace *closed;
    /* The next free slot, for the table's free list; -1 for none. */
    int64_t next_free;
} HeroBox;

static HeroBox *hero_box_chunks[HERO_BOX_CHUNKS];
static hero_box_mutex hero_box_table_lock = HERO_BOX_MUTEX_INIT;
/* Slots ever made, the head of the free list, under the table's lock. */
static int64_t hero_box_made = 0;
static int64_t hero_box_free = -1;
/* Lives begun and not yet closed and emptied: the end of `main`'s first look. */
static _Atomic int64_t hero_box_live = 0;
/* 1 once the program's stop is asked. */
static _Atomic int hero_box_stopped = 0;

/* The place a refusal names, or the bare words where the program gave none. */
_Noreturn static void hero_box_panic(const char *msg, const HeroPlace *where) {
    if (where != NULL) hero_panic_at(msg, where->path, where->name, where->line, where->column);
    hero_panic(msg);
}

static int64_t hero_box_handle(int64_t slot, uint64_t generation) {
    return (int64_t)((generation << HERO_BOX_SLOT_BITS) | (uint64_t)slot);
}

/* The slot a handle names, or NULL for a number no life of this table ever
 * had: zero, a negative, or a slot past those made. */
static HeroBox *hero_box_slot_of(int64_t handle) {
    if (handle <= 0) return NULL;
    uint64_t slot = (uint64_t)handle & (((uint64_t)1 << HERO_BOX_SLOT_BITS) - 1);
    hero_box_take(&hero_box_table_lock);
    int64_t made = hero_box_made;
    hero_box_drop(&hero_box_table_lock);
    if ((int64_t)slot >= made) return NULL;
    return &hero_box_chunks[slot / HERO_BOX_CHUNK][slot % HERO_BOX_CHUNK];
}

static uint64_t hero_box_generation_of(int64_t handle) { return (uint64_t)handle >> HERO_BOX_SLOT_BITS; }

/* A fresh chunk's slots, each with its lock and condition made once. Under the
 * table's lock. */
static void hero_box_make_chunk(int64_t at) {
    HeroBox *chunk = (HeroBox *)hero_calloc_raw(sizeof(HeroBox) * HERO_BOX_CHUNK);
    for (int64_t i = 0; i < HERO_BOX_CHUNK; i += 1) {
#if defined(_WIN32)
        InitializeSRWLock(&chunk[i].lock);
        InitializeConditionVariable(&chunk[i].ready);
#else
        pthread_mutex_init(&chunk[i].lock, NULL);
#if defined(__APPLE__)
        pthread_cond_init(&chunk[i].ready, NULL);
#else
        /* Waited on against the monotonic clock, so a clock set back does not
         * stretch a receive's bound. */
        pthread_condattr_t attr;
        pthread_condattr_init(&attr);
        pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
        pthread_cond_init(&chunk[i].ready, &attr);
        pthread_condattr_destroy(&attr);
#endif
#endif
        chunk[i].next_free = -1;
    }
    hero_box_chunks[at] = chunk;
}

int64_t hero_mailbox_begin(int64_t size, const HeroPlace *where) {
    if (size <= 0) hero_box_panic("a mailbox was begun for a value of no size — a compiler bug", where);
    hero_box_take(&hero_box_table_lock);
    int64_t slot = hero_box_free;
    if (slot >= 0) {
        hero_box_free = hero_box_chunks[slot / HERO_BOX_CHUNK][slot % HERO_BOX_CHUNK].next_free;
    } else {
        if (hero_box_made >= (int64_t)HERO_BOX_CHUNKS * HERO_BOX_CHUNK) {
            hero_box_drop(&hero_box_table_lock);
            hero_box_panic("too many mailboxes open at once — this runtime holds 1,048,576, and a mailbox's "
                           "room comes back once it is closed and every value in it received",
                           where);
        }
        slot = hero_box_made;
        if (slot % HERO_BOX_CHUNK == 0) hero_box_make_chunk(slot / HERO_BOX_CHUNK);
        hero_box_made += 1;
    }
    hero_box_drop(&hero_box_table_lock);

    HeroBox *box = &hero_box_chunks[slot / HERO_BOX_CHUNK][slot % HERO_BOX_CHUNK];
    hero_box_take(&box->lock);
    box->generation += 1;
    box->open = 1;
    box->held = 1;
    box->head = NULL;
    box->tail = NULL;
    box->queued = 0;
    box->waiting = 0;
    box->size = (size_t)size;
    box->begun = where;
    box->closed = NULL;
    uint64_t generation = box->generation;
    hero_box_drop(&box->lock);
    atomic_fetch_add_explicit(&hero_box_live, 1, memory_order_relaxed);
    return hero_box_handle(slot, generation);
}

/* The slot back to the table once its life is closed and empty: its next life
 * takes a new generation, so a copy of this one is told. Under the slot's lock,
 * which it leaves held; a slot past its last generation is never reused. */
static void hero_box_give_back(HeroBox *box, int64_t slot) {
    box->held = 0;
    atomic_fetch_sub_explicit(&hero_box_live, 1, memory_order_relaxed);
    if (box->generation >= HERO_BOX_LAST_GENERATION) return;
    hero_box_take(&hero_box_table_lock);
    box->next_free = hero_box_free;
    hero_box_free = slot;
    hero_box_drop(&hero_box_table_lock);
}

/* The open slot a handle names, its lock held; a handle of no life, or of a
 * life that is over, aborts with `what` saying which operation met it. */
static HeroBox *hero_box_open(int64_t handle, size_t size, const char *what, const HeroPlace *where) {
    HeroBox *box = hero_box_slot_of(handle);
    if (box == NULL) {
        hero_box_panic(what, where);
    }
    hero_box_take(&box->lock);
    if (box->generation != hero_box_generation_of(handle) || !box->held) {
        hero_box_drop(&box->lock);
        return NULL;
    }
    if (box->size != size) {
        hero_box_drop(&box->lock);
        hero_box_panic("a mailbox was handed a value of another size than its own — a compiler bug", where);
    }
    return box;
}

void hero_mailbox_send(int64_t handle, const void *value, int64_t size, HeroDeepFn copy, const HeroPlace *where) {
    if (size <= 0) hero_box_panic("a value of no size was sent — a compiler bug", where);
    HeroBoxNode *node = (HeroBoxNode *)hero_malloc_raw(sizeof(HeroBoxNode) + (size_t)size);
    node->next = NULL;
    /* The copy is made here, on the sender's thread, before the queue is
     * touched: no lock is held while the program's blocks are made. */
    if (copy != NULL) {
        copy(node->value, value);
    } else {
        memcpy(node->value, value, (size_t)size);
    }
    HeroBox *box = hero_box_open(handle, (size_t)size, "a value was sent to a mailbox this program never began", where);
    if (box == NULL || !box->open) {
        hero_box_panic("a value was sent to a mailbox after its close — a mailbox's life ends at its close for "
                       "every copy of it, so nothing sends to it after",
                       where);
    }
    if (box->tail == NULL) {
        box->head = node;
    } else {
        box->tail->next = node;
    }
    box->tail = node;
    box->queued += 1;
#if defined(_WIN32)
    if (box->waiting > 0) WakeConditionVariable(&box->ready);
#else
    if (box->waiting > 0) pthread_cond_signal(&box->ready);
#endif
    hero_box_drop(&box->lock);
}

/* Milliseconds on a clock that never runs back. */
static int64_t hero_box_now_ms(void) {
#if defined(_WIN32)
    return (int64_t)GetTickCount64();
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (int64_t)now.tv_sec * 1000 + (int64_t)(now.tv_nsec / 1000000);
#endif
}

/* One wait on the slot's condition, at most `ms` milliseconds. Its lock held. */
static void hero_box_wait(HeroBox *box, int64_t ms) {
#if defined(_WIN32)
    DWORD slice = ms > 0x7ffffffe ? 0x7ffffffe : (DWORD)ms;
    SleepConditionVariableSRW(&box->ready, &box->lock, slice, 0);
#elif defined(__APPLE__)
    struct timespec rel;
    rel.tv_sec = (time_t)(ms / 1000);
    rel.tv_nsec = (long)((ms % 1000) * 1000000);
    pthread_cond_timedwait_relative_np(&box->ready, &box->lock, &rel);
#else
    struct timespec at;
    clock_gettime(CLOCK_MONOTONIC, &at);
    at.tv_sec += (time_t)(ms / 1000);
    at.tv_nsec += (long)((ms % 1000) * 1000000);
    if (at.tv_nsec >= 1000000000L) {
        at.tv_sec += 1;
        at.tv_nsec -= 1000000000L;
    }
    pthread_cond_timedwait(&box->ready, &box->lock, &at);
#endif
}

/* 0 with the oldest value moved into `*out`; 1 none by the bound, `timeout`;
 * 2 the program's stop asked, `stopped`; 3 the mailbox closed and empty,
 * `closed`. */
int64_t hero_mailbox_receive(int64_t handle, void *out, int64_t size, int64_t within_ms, const HeroPlace *where) {
    if (within_ms < 0) {
        hero_box_panic("a mailbox was asked to wait a time below zero — `within_ms` is how many milliseconds a "
                       "receive may wait, 0 for none",
                       where);
    }
    if (size <= 0) hero_box_panic("a value of no size was received — a compiler bug", where);
    HeroBox *box = hero_box_open(handle, (size_t)size, "a mailbox this program never began was received from", where);
    /* A life that is over, closed and emptied, answers as the closed one it was. */
    if (box == NULL) return 3;
    int64_t until = within_ms > INT64_MAX - hero_box_now_ms() ? INT64_MAX : hero_box_now_ms() + within_ms;
    box->waiting += 1;
    for (;;) {
        if (box->head != NULL) {
            HeroBoxNode *node = box->head;
            box->head = node->next;
            if (box->head == NULL) box->tail = NULL;
            box->queued -= 1;
            box->waiting -= 1;
            int64_t slot = (int64_t)((uint64_t)handle & (((uint64_t)1 << HERO_BOX_SLOT_BITS) - 1));
            if (!box->open && box->queued == 0 && box->waiting == 0) hero_box_give_back(box, slot);
            hero_box_drop(&box->lock);
            /* Moved, never copied a second time: the value's blocks were made
             * for this receiver alone by the send. */
            memcpy(out, node->value, (size_t)size);
            hero_free_raw(node);
            return 0;
        }
        if (!box->open) {
            box->waiting -= 1;
            int64_t slot = (int64_t)((uint64_t)handle & (((uint64_t)1 << HERO_BOX_SLOT_BITS) - 1));
            if (box->waiting == 0 && box->held) hero_box_give_back(box, slot);
            hero_box_drop(&box->lock);
            return 3;
        }
        if (atomic_load(&hero_box_stopped)) {
            box->waiting -= 1;
            hero_box_drop(&box->lock);
            return 2;
        }
        int64_t left = until - hero_box_now_ms();
        if (left <= 0) {
            box->waiting -= 1;
            hero_box_drop(&box->lock);
            return 1;
        }
        hero_box_wait(box, left);
    }
}

void hero_mailbox_close(int64_t handle, int64_t size, const HeroPlace *where) {
    HeroBox *box = hero_box_open(handle, (size_t)size, "a mailbox this program never began was closed", where);
    if (box == NULL || !box->open) {
        if (box != NULL) hero_box_drop(&box->lock);
        hero_box_panic("a mailbox was closed twice — its life ends at its first close, for every copy of it", where);
    }
    box->open = 0;
    box->closed = where;
    int64_t slot = (int64_t)((uint64_t)handle & (((uint64_t)1 << HERO_BOX_SLOT_BITS) - 1));
    if (box->queued == 0 && box->waiting == 0) {
        hero_box_give_back(box, slot);
    } else {
#if defined(_WIN32)
        WakeAllConditionVariable(&box->ready);
#else
        pthread_cond_broadcast(&box->ready);
#endif
    }
    hero_box_drop(&box->lock);
}

/* The platform layer's word that the program's stop was asked: every receive
 * waiting wakes and answers `stopped`. Never inside a signal handler: it takes
 * each slot's lock in turn, and the layer's lock may be held around it, which
 * no thread here takes. */
void hero_layer_stop_heard(void) {
    if (atomic_exchange(&hero_box_stopped, 1) != 0) return;
    hero_box_take(&hero_box_table_lock);
    int64_t made = hero_box_made;
    hero_box_drop(&hero_box_table_lock);
    for (int64_t slot = 0; slot < made; slot += 1) {
        HeroBox *box = &hero_box_chunks[slot / HERO_BOX_CHUNK][slot % HERO_BOX_CHUNK];
        hero_box_take(&box->lock);
#if defined(_WIN32)
        if (box->waiting > 0) WakeAllConditionVariable(&box->ready);
#else
        if (box->waiting > 0) pthread_cond_broadcast(&box->ready);
#endif
        hero_box_drop(&box->lock);
    }
}

/* -- the failures a receive and a start answer ------------------------------ */

HeroFailure hero_failure_timeout(void) {
    HERO_STR_STATIC(code, "timeout");
    HERO_STR_STATIC(msg, "no value arrived in the mailbox within the time given");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_stopped(void) {
    HERO_STR_STATIC(code, "stopped");
    HERO_STR_STATIC(msg, "the program's stop was asked, and no value was waiting in the mailbox");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_closed(void) {
    HERO_STR_STATIC(code, "closed");
    HERO_STR_STATIC(msg, "the mailbox was closed, and every value sent to it has been received");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_refused(void) {
    HERO_STR_STATIC(code, "refused");
    HERO_STR_STATIC(msg, "no thread could be started: the system refused one, or 256 of this program's run already");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

/* -- the copies -------------------------------------------------------------- */

void hero_deep_str(void *dst, const void *src) {
    HeroStr s = *(const HeroStr *)src;
    hero_str_require(s);
    *(HeroStr *)dst = hero_str_from_bytes(s.ptr, s.len);
}

void hero_deep_failure(void *dst, const void *src) {
    const HeroFailure *f = (const HeroFailure *)src;
    HeroFailure *out = (HeroFailure *)dst;
    hero_deep_str(&out->code, &f->code);
    hero_deep_str(&out->msg, &f->msg);
}

/* `[T]` whole: a block of its own and each element copied by `each`, or byte
 * for byte where `each` is NULL (a number, a function, a handle's number). */
void hero_deep_array(void *dst, const void *src, HeroDeepFn each) {
    const HeroArrayHeader *a = *(HeroArrayHeader *const *)src;
    hero_array_require(a);
    HeroArrayHeader *out = hero_array_new(a->elem, a->len > 0 ? a->len : 1);
    const unsigned char *from = hero_array_data_const(a);
    unsigned char *to = hero_array_data(out);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i += 1) {
        if (each != NULL) {
            each(to + (size_t)i * size, from + (size_t)i * size);
        } else {
            memcpy(to + (size_t)i * size, from + (size_t)i * size, size);
        }
    }
    out->len = a->len;
    *(HeroArrayHeader **)dst = out;
}

/* `{K: V}` whole, its entries in the order they were first written: each key
 * and value copied into a temporary of the map's own, put (which takes a
 * reference of its own), and the temporary's released. */
void hero_deep_map(void *dst, const void *src, HeroDeepFn key, HeroDeepFn value) {
    const HeroMapHeader *m = *(HeroMapHeader *const *)src;
    hero_map_require(m);
    HeroMapHeader *out = hero_map_new(m->key, m->val, m->len);
    unsigned char *k = (unsigned char *)hero_malloc_raw(m->key->size);
    unsigned char *v = (unsigned char *)hero_malloc_raw(m->val->size);
    for (int64_t at = 0; at < m->len; at += 1) {
        int64_t bucket = hero_map_entry_bucket(m, at);
        if (key != NULL) {
            key(k, hero_map_key_at_const(m, bucket));
        } else {
            memcpy(k, hero_map_key_at_const(m, bucket), m->key->size);
        }
        if (value != NULL) {
            value(v, hero_map_val_at_const(m, bucket));
        } else {
            memcpy(v, hero_map_val_at_const(m, bucket), m->val->size);
        }
        hero_map_put(out, k, v);
        m->key->drop(k);
        m->val->drop(v);
    }
    hero_free_raw(k);
    hero_free_raw(v);
    *(HeroMapHeader **)dst = out;
}

/* -- the language's threads ---------------------------------------------------
 *
 * The program's C fills a box (`HeroThreadBox` and what follows it: the
 * function, the argument copied whole, room for the answer), starts it here,
 * and reads the answer back from it at the wait; this file and `parts/spawn.c`
 * never look inside it. The box is the runtime's own memory, weighed by no
 * counter, as `parts/spawn_str.c`'s is; the values in it are the program's. */

static int64_t hero_thread_lang_run(int64_t number) {
    HeroThreadBox *box = (HeroThreadBox *)(uintptr_t)number;
    box->run(box);
    return number;
}

#define HERO_THREAD_SLOT_BITS 16

int64_t hero_thread_begin(HeroThreadBox *box, int64_t bytes, const HeroPlace *where) {
    if (box == NULL || box->run == NULL) hero_box_panic("a thread was started with no function to run", where);
    if (bytes < 0) {
        hero_box_panic("a thread was asked for a negative number of bytes of stack — `bytes` is the least "
                       "size of its stack",
                       where);
    }
    int64_t why = 0;
    int64_t at = hero_spawn_start(hero_thread_lang_run, (int64_t)(uintptr_t)box, (size_t)bytes, HERO_SPAWN_LANG,
                                  &why, (uintptr_t)box->body, (uintptr_t)__builtin_return_address(0));
    if (at < 0) return -1;
    HERO_SPAWN_TAKE();
    hero_spawn_slots[at].place = where;
    uint64_t generation = hero_spawn_slots[at].generation;
    HERO_SPAWN_DROP();
    return (int64_t)((generation << HERO_THREAD_SLOT_BITS) | (uint64_t)at);
}

HeroThreadBox *hero_thread_end(int64_t handle, const HeroPlace *where) {
    int64_t at = (int64_t)((uint64_t)handle & (((uint64_t)1 << HERO_THREAD_SLOT_BITS) - 1));
    uint64_t generation = (uint64_t)handle >> HERO_THREAD_SLOT_BITS;
    if (handle <= 0 || at >= HERO_SPAWN_SLOTS) {
        hero_box_panic("a thread this program never started was waited for", where);
    }
    HERO_SPAWN_TAKE();
    int fresh = hero_spawn_slots[at].state != 0 && hero_spawn_slots[at].kind == HERO_SPAWN_LANG &&
                hero_spawn_slots[at].generation == generation;
    HERO_SPAWN_DROP();
    if (!fresh) {
        hero_box_panic("a thread was waited for twice — `t.wait()` ends the thread's life, and every copy of it "
                       "with it",
                       where);
    }
    return (HeroThreadBox *)(uintptr_t)hero_spawn_join(at, HERO_SPAWN_LANG);
}

void hero_thread_box_free(HeroThreadBox *box) { hero_free_raw(box); }

void *hero_thread_box_new(int64_t size) {
    if (size < (int64_t)sizeof(HeroThreadBox)) hero_panic("a thread's box smaller than its head — a compiler bug");
    return hero_malloc_raw((size_t)size);
}

/* -- the end of `main` --------------------------------------------------------
 *
 * After the threads (`hero_spawn_check_joined`), before any balance: a mailbox
 * still open, or closed with values nobody received, is the program's mistake,
 * told with how many and where each was begun, the first eight by slot. */
#define HERO_BOX_NAMED 8

static void hero_mailbox_check_closed(void) {
    if (atomic_load(&hero_box_live) == 0) return;
    int64_t open = 0;
    int64_t unreceived = 0;
    int64_t named = 0;
    const HeroBox *told[HERO_BOX_NAMED];
    hero_box_take(&hero_box_table_lock);
    int64_t made = hero_box_made;
    hero_box_drop(&hero_box_table_lock);
    for (int64_t slot = 0; slot < made; slot += 1) {
        HeroBox *box = &hero_box_chunks[slot / HERO_BOX_CHUNK][slot % HERO_BOX_CHUNK];
        hero_box_take(&box->lock);
        if (box->held) {
            if (box->open) open += 1;
            unreceived += box->queued;
            if (named < HERO_BOX_NAMED) told[named++] = box;
        }
        hero_box_drop(&box->lock);
    }
    if (open == 0 && unreceived == 0) return;
    hero_report_begin();
    hero_report_printf("panic: %lld mailbox(es) never closed and %lld value(s) sent and never received when `main` "
                       "returned — every mailbox a program begins is closed, and every value sent to one is "
                       "received, before `main` returns\n",
                       (long long)open, (long long)unreceived);
    for (int64_t k = 0; k < named; k += 1) {
        const HeroBox *box = told[k];
        hero_report_printf("  a mailbox %s, %lld value(s) in it", box->open ? "never closed" : "closed",
                           (long long)box->queued);
        if (box->begun != NULL) {
            hero_report_printf(", begun at %s:%lld:%lld, in %s", box->begun->path, (long long)box->begun->line,
                               (long long)box->begun->column, box->begun->name);
        }
        hero_stderr_put("\n", 1);
    }
    hero_abort();
}

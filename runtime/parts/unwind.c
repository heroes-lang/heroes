/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/unwind.c — the values a C jump abandons, collected by an unwind of
 * the frames it leaves and released when the C call holding the jump's target
 * returns into Heroes code (panel 258's R2, ratified by the author's reading
 * on 2026-11-27; design.md §1.11, §1.12). Needs `parts/callback.c`.
 *
 * THE FACT. A C library may `longjmp` over the frames of a Heroes callback
 * (libpng's `png_error`, Lua's `luaL_error`, a binding's own save point around
 * a decode): the jump runs no code, so what those frames owned stays owned by
 * nobody, a leak the exit audit forgives (`parts/callback.c`). The sitting
 * measured, with real libpng, that a release AT the jump frees what C still
 * reads at its catch (a message the callback lent `png_error`, read after the
 * landing: an empty message at exit 0, a heap use after free under ASan), and
 * that the one moment the FFI's contract says C has handed every lend back is
 * the return of the extern call holding the catch. So the values are
 * COLLECTED at the jump and RELEASED at that return.
 *
 * THE COLLECTOR. Where a program hands a function to a C library's own group
 * (`selfhost/emit/callback_guard.hero`, the scope), every counted slot of a
 * function C may call back, and of every function it reaches, carries a
 * cleanup (`hero_unwind_str` and its siblings in `heroes_runtime.h`, or a
 * type's own `_unwound`), and its unit is built `-fexceptions`, so the
 * platform's unwinder runs them when it unwinds the frame. The jump's hook,
 * on Darwin's and on every POSIX system's route (`parts/rebind_jump.c`,
 * `parts/sigjump_save.c`), calls `hero_unwind_jump` before the C library's
 * own jump: it walks the frames above it once, and where one of them stands
 * at exactly the stack pointer the jump will install, the jump's target is a
 * frame of this chain and the frames below it are abandoned; it then runs
 * `_Unwind_ForcedUnwind`, whose stop makes the real jump at that frame. Each
 * cleanup the unwind runs copies its slot's value onto this thread's group,
 * owning it; on every other path a cleanup reads one word and returns.
 *
 * WHERE IT DOES NOTHING, AND THE PROGRAM IS AS IT WAS (the leak, forgiven):
 * a jump made inside a signal handler (no unwind is async-signal-safe); a
 * jump whose target no frame of the chain stands at exactly (route A's
 * record, which names a frame near the save and not the stack pointer; a
 * frame that moved its stack pointer after the save; a switch of stacks made
 * by a jump, OpenSSL's fibres' `_longjmp` among them, whose target lies on
 * another chain, so a coroutine's suspended frames are never taken for
 * abandoned ones); a frame built without unwind information (the walk stops
 * at it and the jump is made there); a jump this runtime never sees (an
 * inline `__builtin_longjmp`, assembly), a C++ exception and glibc's
 * `pthread_exit`, which run the cleanups while `hero_unwind_running` reads 0,
 * so they collect nothing; and Windows, which has no hook (UCRT's `longjmp`).
 *
 * THE RELEASE. The walk keeps, for the group, the frame pointer of the frame
 * the jump lands in and of up to `HERO_UNWIND_ABOVE` frames above it. Every
 * call of a C library's group in such a program is followed by
 * `hero_unwind_returned(__builtin_frame_address(0))`: a Heroes frame among
 * those is past the call holding the catch, every frame below it having
 * returned, so the group is released there, never before. A frame is told by
 * its own pointer, never by an order of addresses, which a switch of stacks
 * can falsify (panel 259). A group whose catch is abandoned in turn by a later
 * unwind, its landing frame being one that unwind leaves, by the same
 * equality, joins that unwind's group; and whatever still waits when the
 * program ends is released before the exit audit reads the balance
 * (`hero_unwind_release_all`, `parts/alloc.c`), since no C call is in
 * progress then.
 *
 * WHAT IT DOES NOT RELEASE, by panel 259's sentence in spec § 13: a value of
 * more than 4,096 bytes as the emitter counts them, or holding a `partial`
 * record, lives in the room off the frame (`parts/frame.c`), and a local lent
 * whole lives in a region (`parts/lend.c`); both stay held until the program
 * ends, counted against the bound, and the room's abort still says how many
 * callbacks have not returned, an unwound one among them (it did not
 * return). A function holding one of them, or a reference across a call
 * (`selfhost/ir/held.hero`), carries `hero_unwind_residue`, and an unwind over
 * it keeps the exit audit forgiving, as it was; and so does a frame with
 * cleanups none of which ran, which the stop finds by the frame's language
 * table and its mark (`hero_unwind_stop`): clang compiles a call to a function
 * C's header declares `nothrow` (glibc's `longjmp`, which a header's inline
 * helper calls) as one that cannot unwind, with no landing pad, and the
 * unwinder passes that frame by. Nor does it end a handle or a
 * lease: the gates keep the aborts spec § 13 promises, saying the callbacks
 * C jumped over. */

#if !defined(_WIN32)
#include <pthread.h>
#include <unwind.h>
#endif

/* `parts/alloc.c`, later in this unit: the one `malloc` neither counter
 * weighs, so the list's own memory is no block of the program's. */
static void *hero_malloc_raw(size_t size);
static void hero_free_raw(void *p);

/* The jumps a hook hands `hero_unwind_jump`, which the stop makes. */
#define HERO_UNWIND_LONGJMP 0
#define HERO_UNWIND__LONGJMP 1
#define HERO_UNWIND_SIGLONGJMP 2
#define HERO_UNWIND_LONGJMP_CHK 3

/* Threads now inside an unwind this runtime began, read by every cleanup. */
int64_t hero_unwind_running = 0;

/* Groups waiting for the call holding their catch to return, read after every
 * call of a C library's group, under no lock. */
static int64_t hero_unwind_waiting = 0;

/* Whether a thunk of a program that unwinds was entered: set by its mark's
 * initialiser, which `selfhost/emit/unwind_scope.hero` writes in such a
 * program alone, so no other program ever walks a stack here. */
static int64_t hero_unwind_scoped = 0;

/* How many frames at and above the catch a group knows by their pointer. A
 * Heroes frame making the call that holds the catch stands further up only
 * behind as many C frames, and its group then waits for the program's end. */
#define HERO_UNWIND_ABOVE 64

/* How many abandoned frames a walk remembers by their stack pointer, to find
 * the groups whose catch they were. */
#define HERO_UNWIND_LEFT 256

/* One value a cleanup kept: its slot's bytes and the release that owns them. */
typedef struct HeroUnwoundValue {
    struct HeroUnwoundValue *next;
    void (*release)(void *);
    size_t size;
} HeroUnwoundValue;

#define HERO_UNWOUND_HEAD ((sizeof(HeroUnwoundValue) + 15) & ~(size_t)15)

/* The values one unwind kept, and the frames whose return releases them. */
typedef struct HeroUnwoundGroup {
    struct HeroUnwoundGroup *next;
    HeroUnwoundValue *values;
    uintptr_t landing;
    int frames_n;
    uintptr_t frames[HERO_UNWIND_ABOVE];
} HeroUnwoundGroup;

#if defined(_WIN32)
typedef SRWLOCK hero_unwind_mutex;
static hero_unwind_mutex hero_unwind_lock = SRWLOCK_INIT;
static void hero_unwind_take(void) { AcquireSRWLockExclusive(&hero_unwind_lock); }
static void hero_unwind_drop(void) { ReleaseSRWLockExclusive(&hero_unwind_lock); }
#else
static pthread_mutex_t hero_unwind_lock = PTHREAD_MUTEX_INITIALIZER;
static void hero_unwind_take(void) { pthread_mutex_lock(&hero_unwind_lock); }
static void hero_unwind_drop(void) { pthread_mutex_unlock(&hero_unwind_lock); }
#endif

/* Every group waiting, the newest first, under the lock. */
static HeroUnwoundGroup *hero_unwind_groups = NULL;

/* This thread's unwind, while one runs: what the stop jumps to, and the group
 * the cleanups fill. Never on the stack the unwind discards, and so neither is
 * the exception object, which the unwinder reads at every frame. */
typedef struct {
    int on;
    int kind;
    void *env;
    int value;
    uintptr_t target;
    HeroUnwoundGroup *group;
    /* The frame the unwind last walked: whether it has cleanups (a language
     * table), whether one of its marks ran, and whether a frame with cleanups
     * was left with none run (`hero_unwind_stop`). */
    bool frame_tabled;
    bool frame_reached;
    bool missed;
#if !defined(_WIN32)
    struct _Unwind_Exception exception;
#endif
} HeroUnwindThread;

static _Thread_local HeroUnwindThread hero_unwind_this;

/* A value's bytes, past its header. */
static void *hero_unwind_bytes(HeroUnwoundValue *v) { return (char *)v + HERO_UNWOUND_HEAD; }

/* The group's values, each released by its own release and given back. */
static void hero_unwind_release_group(HeroUnwoundGroup *g) {
    HeroUnwoundValue *v = g->values;
    while (v != NULL) {
        HeroUnwoundValue *next = v->next;
        v->release(hero_unwind_bytes(v));
        hero_free_raw(v);
        v = next;
    }
    hero_free_raw(g);
}

void hero_unwind_kept(void *slot, size_t size, void (*release)(void *)) {
    HeroUnwindThread *me = &hero_unwind_this;
    if (!me->on || slot == NULL || release == NULL) return;
    HeroUnwoundValue *v = (HeroUnwoundValue *)hero_malloc_raw(HERO_UNWOUND_HEAD + size);
    v->release = release;
    v->size = size;
    memcpy(hero_unwind_bytes(v), slot, size);
    v->next = me->group->values;
    me->group->values = v;
}

static void hero_unwind_release_str(void *slot) { hero_str_release_at((const HeroStr *)slot); }
static void hero_unwind_release_array(void *slot) { hero_array_release_at((HeroArrayHeader *const *)slot); }
static void hero_unwind_release_map(void *slot) { hero_map_release_at((HeroMapHeader *const *)slot); }
static void hero_unwind_release_failure(void *slot) { hero_failure_release((HeroFailure *)slot); }
static void hero_unwind_release_mail(void *slot) { hero_mail_release_at((HeroMail **)slot); }

void hero_unwind_kept_str(void *slot) { hero_unwind_kept(slot, sizeof(HeroStr), hero_unwind_release_str); }
void hero_unwind_kept_array(void *slot) {
    hero_unwind_kept(slot, sizeof(HeroArrayHeader *), hero_unwind_release_array);
}
void hero_unwind_kept_map(void *slot) { hero_unwind_kept(slot, sizeof(HeroMapHeader *), hero_unwind_release_map); }
void hero_unwind_kept_failure(void *slot) {
    hero_unwind_kept(slot, sizeof(HeroFailure), hero_unwind_release_failure);
}
void hero_unwind_kept_mail(void *slot) { hero_unwind_kept(slot, sizeof(HeroMail *), hero_unwind_release_mail); }

/* A thunk the unwind leaves: its callback no longer stands unfinished, its
 * values being collected, and it is counted among those that never returned
 * (`parts/callback.c`). */
void hero_unwind_unwound(void) {
    if (!hero_unwind_this.on) return;
    hero_unwind_this.frame_reached = true;
    hero_callback_left();
    __atomic_add_fetch(&hero_callbacks_unwound, 1, __ATOMIC_RELAXED);
}

/* A frame the unwind leaves whose cleanups ran: every one in scope at its
 * call runs in the one landing pad, so its mark among them says all did. */
void hero_unwind_framed(void) {
    if (!hero_unwind_this.on) return;
    hero_unwind_this.frame_reached = true;
}

/* A frame the unwind leaves holding what it does not release. */
void hero_unwind_held(void) {
    if (!hero_unwind_this.on) return;
    hero_unwind_this.frame_reached = true;
    __atomic_add_fetch(&hero_callbacks_residue, 1, __ATOMIC_RELAXED);
}

/* A thunk of a program that unwinds was entered: its mark's initialiser,
 * answering the mark's 0. */
char hero_unwind_handed(void) {
    if (__atomic_load_n(&hero_unwind_scoped, __ATOMIC_RELAXED) == 0) {
        __atomic_store_n(&hero_unwind_scoped, 1, __ATOMIC_RELAXED);
    }
    return 0;
}

/* Whether `frame` is one of the group's. */
static bool hero_unwind_names(const HeroUnwoundGroup *g, uintptr_t frame) {
    for (int i = 0; i < g->frames_n; i += 1) {
        if (g->frames[i] == frame) return true;
    }
    return false;
}

/* The slow half of `hero_unwind_returned`: every group a frame of which made
 * the call that just returned, taken off the list under the lock and released
 * outside it, since a release takes the allocator's. */
__attribute__((noinline)) static void hero_unwind_settle(uintptr_t frame) {
    HeroUnwoundGroup *done = NULL;
    hero_unwind_take();
    HeroUnwoundGroup **at = &hero_unwind_groups;
    while (*at != NULL) {
        HeroUnwoundGroup *g = *at;
        if (hero_unwind_names(g, frame)) {
            *at = g->next;
            g->next = done;
            done = g;
            __atomic_sub_fetch(&hero_unwind_waiting, 1, __ATOMIC_RELEASE);
        } else {
            at = &g->next;
        }
    }
    hero_unwind_drop();
    while (done != NULL) {
        HeroUnwoundGroup *next = done->next;
        hero_unwind_release_group(done);
        done = next;
    }
}

void hero_unwind_returned(void *frame) {
    if (__atomic_load_n(&hero_unwind_waiting, __ATOMIC_ACQUIRE) == 0) return;
    hero_unwind_settle((uintptr_t)frame);
}

/* What still waits at the end of the program: no C call is in progress, so
 * every catch has returned or never will. */
static void hero_unwind_release_all(void) {
    if (__atomic_load_n(&hero_unwind_waiting, __ATOMIC_ACQUIRE) == 0) return;
    hero_unwind_take();
    HeroUnwoundGroup *all = hero_unwind_groups;
    hero_unwind_groups = NULL;
    __atomic_store_n(&hero_unwind_waiting, 0, __ATOMIC_RELEASE);
    hero_unwind_drop();
    while (all != NULL) {
        HeroUnwoundGroup *next = all->next;
        hero_unwind_release_group(all);
        all = next;
    }
}

#if !defined(_WIN32) && (defined(__aarch64__) || defined(__x86_64__))

/* The frame pointer's register in the unwinder's numbering: x29, rbp. */
#if defined(__aarch64__)
#define HERO_UNWIND_FP 29
#else
#define HERO_UNWIND_FP 6
#endif

/* "HEROUNW\0": the exception class of an unwind this runtime began. */
#define HERO_UNWIND_CLASS ((uint64_t)0x4845524f554e5700ull)

/* `parts/rebind_jump.c` or `parts/sigjump_save.c`, later in this unit: the
 * C library's own jump of `kind`, after what the platform's route does before
 * any jump leaves frames (AddressSanitizer's shadow); and that clearing alone,
 * from the caller's frame to the stack's top, where the stack is one the
 * route can clear. */
__attribute__((no_sanitize("address"))) static _Noreturn void hero_unwind_go(int kind, void *env, int value);
static void hero_unwind_clear(void);

/* And `size` bytes at `at` copied by the kernel, never loaded, so an address
 * no page backs is an answer and never a fault; false where it refuses. */
static bool hero_unwind_peek(uintptr_t at, void *into, size_t size);

/* WHETHER THE INSTRUCTION BEFORE `ip` IS A CALL, read by the kernel: a frame
 * the walk meets at any other address was stopped by a signal in the middle
 * of its code, not at a call into C, and its slots may hold a value half
 * stored (an old one being replaced, a temporary owning it). A forced unwind
 * there would hand a cleanup a slot mid-store, and that is how this hook's
 * first landing on main corrupted a watchdog's program whose handler a raw
 * system call installed, unseen by the routes (`wdr`, panel 262's critic: a
 * release after the last reference, and SIGSEGV, 11 runs of 34). An answer
 * this cannot give is no. */
static bool hero_unwind_after_call(uintptr_t ip) {
#if defined(__aarch64__)
    uint32_t w = 0;
    if (ip < 4 || !hero_unwind_peek(ip - 4, &w, sizeof w)) return false;
    if ((w & 0xFC000000u) == 0x94000000u) return true;               /* bl */
    if ((w & 0xFFFFFC1Fu) == 0xD63F0000u) return true;               /* blr */
    return (w & 0xFEFFF800u) == 0xD63F0800u;                          /* blraa, blrab, blraaz, blrabz */
#else
    unsigned char b[8];
    if (ip < 8 || !hero_unwind_peek(ip - 8, b, sizeof b)) return false;
    /* b[7] is the byte before ip. */
    if (b[3] == 0xE8) return true;                                    /* call rel32 */
    unsigned char m2 = b[7], m3 = b[6], m6 = b[3], m7 = b[2];
    if (b[6] == 0xFF && (m2 & 0xF8) == 0xD0) return true;             /* call reg */
    if (b[5] == 0xFF && (m3 & 0x38) == 0x10 && (m3 & 0xC0) == 0x40 && (m3 & 7) != 4) return true; /* [reg+d8] */
    if (b[4] == 0xFF && (b[5] & 0x38) == 0x10 && (b[5] & 0xC0) == 0x40 && (b[5] & 7) == 4) return true; /* [sib+d8] */
    if (b[2] == 0xFF && (m6 & 0x38) == 0x10 && ((m6 & 0xC7) == 0x05 || ((m6 & 0xC0) == 0x80 && (m6 & 7) != 4))) {
        return true;                                                  /* [rip+d32], [reg+d32] */
    }
    if (b[1] == 0xFF && (m7 & 0x38) == 0x10 && (m7 & 0xC0) == 0x80 && (m7 & 7) == 4) return true; /* [sib+d32] */
    if (b[5] == 0xFF && (b[6] & 0x38) == 0x10 && (b[6] & 0xC0) == 0 && (b[6] & 7) != 4 && (b[6] & 7) != 5) {
        return b[4] >= 0x40 && b[4] <= 0x4F;                          /* rex, [reg] */
    }
    return false;
#endif
}

/* One walk of the frames above the hook, before anything is unwound. The
 * unwinder reads a frame's own stack pointer at its call as `_Unwind_GetCFA`
 * on Darwin's and on GNU's unwinder alike (glibc's cancellation compares the
 * same number with a `jmp_buf`'s); the frame the jump lands in stands at the
 * target exactly, and every frame below it stands lower. */
typedef struct {
    uintptr_t target;
    uintptr_t lo;
    uintptr_t hi;
    bool found;
    bool passed;
    bool stopped;
    int left_n;
    uintptr_t left[HERO_UNWIND_LEFT];
    int frames_n;
    uintptr_t frames[HERO_UNWIND_ABOVE];
} HeroUnwindWalk;

static _Unwind_Reason_Code hero_unwind_walked(struct _Unwind_Context *ctx, void *arg) {
    HeroUnwindWalk *walk = (HeroUnwindWalk *)arg;
    uintptr_t sp = (uintptr_t)_Unwind_GetCFA(ctx);
    /* Below the target every frame must be on the thread's own stack and
     * stopped at a call: a signal's frame (the unwinder's own flag) or a
     * frame stopped anywhere else says the jump came out of a handler, and
     * nothing is unwound (`hero_unwind_after_call`). */
    if (!walk->found) {
        int signalled = 0;
        uintptr_t ip = (uintptr_t)_Unwind_GetIPInfo(ctx, &signalled);
        if (signalled != 0 || sp < walk->lo || sp >= walk->hi || !hero_unwind_after_call(ip)) {
            walk->stopped = true;
            return _URC_NORMAL_STOP;
        }
    } else if (sp < walk->lo || sp >= walk->hi) {
        return _URC_NORMAL_STOP;
    }
    if (!walk->found) {
        if (sp == walk->target) {
            walk->found = true;
        } else if (sp > walk->target) {
            walk->passed = true;
            return _URC_NORMAL_STOP;
        } else {
            if (walk->left_n < HERO_UNWIND_LEFT) walk->left[walk->left_n] = sp;
            walk->left_n += 1;
            return _URC_NO_REASON;
        }
    }
    if (walk->frames_n >= HERO_UNWIND_ABOVE) return _URC_NORMAL_STOP;
    walk->frames[walk->frames_n] = (uintptr_t)_Unwind_GetGR(ctx, HERO_UNWIND_FP);
    walk->frames_n += 1;
    return _URC_NO_REASON;
}

/* The end of this thread's unwind: the group published, waiting for its
 * catch's call to return, unless the unwind kept nothing. */
static void hero_unwind_finish(HeroUnwindThread *me) {
    HeroUnwoundGroup *g = me->group;
    me->group = NULL;
    me->on = 0;
    __atomic_sub_fetch(&hero_unwind_running, 1, __ATOMIC_RELAXED);
    if (g == NULL) return;
    if (g->values == NULL) {
        hero_free_raw(g);
        return;
    }
    hero_unwind_take();
    g->next = hero_unwind_groups;
    hero_unwind_groups = g;
    __atomic_add_fetch(&hero_unwind_waiting, 1, __ATOMIC_RELEASE);
    hero_unwind_drop();
}

/* The stop, asked before each frame's cleanups run: at the frame the jump
 * lands in, or where the unwinder finds no more frames, the C library's own
 * jump is made, and no cleanup of that frame or any above it runs. */
/* Not instrumented under AddressSanitizer, as the jumps' wrappers are not
 * (defect d-lane-sig-family-10): clang would put `__asan_handle_no_return`
 * before the jump below, which on a coroutine's stack clears nothing and warns
 * into a correct program's stderr; `hero_unwind_go` clears what the route can. */
__attribute__((no_sanitize("address"))) static _Unwind_Reason_Code hero_unwind_stop(int version, _Unwind_Action actions, _Unwind_Exception_Class kind,
                                            struct _Unwind_Exception *exception, struct _Unwind_Context *ctx,
                                            void *param) {
    (void)version;
    (void)kind;
    (void)exception;
    (void)param;
    HeroUnwindThread *me = &hero_unwind_this;
    /* The frame walked before this one is done: where it had cleanups and no
     * mark of it ran, its call had no landing pad (a callee C's header
     * declares `nothrow`, compiled as a call that cannot unwind) and what it
     * held stays held, so the exit audit must forgive as it did before. */
    if (me->frame_tabled && !me->frame_reached) me->missed = true;
    if ((actions & _UA_END_OF_STACK) != 0 || (uintptr_t)_Unwind_GetCFA(ctx) >= me->target) {
        int jump = me->kind;
        void *env = me->env;
        int value = me->value;
        if (me->missed) __atomic_add_fetch(&hero_callbacks_residue, 1, __ATOMIC_RELAXED);
        hero_unwind_finish(me);
        hero_unwind_go(jump, env, value);
    }
    me->frame_tabled = _Unwind_GetLanguageSpecificData(ctx) != 0;
    me->frame_reached = false;
    return _URC_NO_REASON;
}

/* The groups whose catch this unwind abandons, every frame they landed in
 * being one it leaves, moved into its own group: their values wait for this
 * catch's call instead. */
static void hero_unwind_adopt(HeroUnwoundGroup *into, const HeroUnwindWalk *walk) {
    int n = walk->left_n < HERO_UNWIND_LEFT ? walk->left_n : HERO_UNWIND_LEFT;
    hero_unwind_take();
    HeroUnwoundGroup **at = &hero_unwind_groups;
    while (*at != NULL) {
        HeroUnwoundGroup *g = *at;
        bool left = false;
        for (int i = 0; i < n && !left; i += 1) left = walk->left[i] == g->landing;
        if (!left) {
            at = &g->next;
            continue;
        }
        *at = g->next;
        __atomic_sub_fetch(&hero_unwind_waiting, 1, __ATOMIC_RELEASE);
        HeroUnwoundValue *v = g->values;
        while (v != NULL) {
            HeroUnwoundValue *next = v->next;
            v->next = into->values;
            into->values = v;
            v = next;
        }
        hero_free_raw(g);
    }
    hero_unwind_drop();
}

/* WHETHER THE FRAMES FROM `fp` TO THE TARGET ARE ONE CHAIN ON THE THREAD'S OWN
 * STACK, asked before the unwinder reads any of them: each frame record's
 * saved frame pointer above the one before it and below the stack's top, up
 * to a record at or past the target. The unwinder steps frames by reading
 * memory their records and its tables name, and a coroutine's first frame,
 * whose record a library writes as it likes, sent it to address -8 (`n2` and
 * `escape_heap`, panel 262's programs: a jump from a coroutine's stack to its
 * host, ASan's SEGV inside `libunwind::UnwindCursor::step`, 3 runs of 3):
 * a chain that leaves the stack, turns back or ends is no unwind. A frame
 * kept with no record (x86-64's `-fomit-frame-pointer`) breaks the chain the
 * same way, the safe side. */
static bool hero_unwind_chained(uintptr_t fp, uintptr_t target, uintptr_t lo, uintptr_t hi) {
    for (int steps = 0; steps < 100000; steps += 1) {
        if (fp >= target) return true;
        if (fp < lo || fp + 2 * sizeof(uintptr_t) > hi || (fp & (sizeof(uintptr_t) - 1)) != 0) return false;
        uintptr_t next = ((const uintptr_t *)fp)[0];
        if (next <= fp || next >= hi) return false;
        fp = next;
    }
    return false;
}

/* THE HOOK, before a jump of the setjmp family made outside every signal
 * handler, the target decoded (`target`, the stack pointer the jump will
 * install): where the program unwinds and a callback stands on this thread,
 * and a frame of the chain stands at the target exactly, the frames below it
 * are unwound and the jump is made from the stop, never returning here. In
 * every other case it returns, and the caller jumps as it always did. */
__attribute__((noinline)) static void hero_unwind_jump(void *env, int value, int kind, uintptr_t target, uintptr_t lo,
                                                     uintptr_t hi) {
    if (__atomic_load_n(&hero_unwind_scoped, __ATOMIC_RELAXED) == 0 || env == NULL || target == 0) return;
    _Atomic int64_t *mine = hero_callback_mine;
    if (mine == NULL || atomic_load_explicit(mine, memory_order_relaxed) <= 0) return;
    HeroUnwindThread *me = &hero_unwind_this;
    if (me->on) return;
    if (!hero_unwind_chained((uintptr_t)__builtin_frame_address(0), target, lo, hi)) return;
    HeroUnwindWalk walk;
    memset(&walk, 0, sizeof walk);
    walk.target = target;
    walk.lo = lo;
    walk.hi = hi;
    _Unwind_Backtrace(hero_unwind_walked, &walk);
    if (walk.stopped || !walk.found || walk.frames_n == 0) return;
    HeroUnwoundGroup *g = (HeroUnwoundGroup *)hero_malloc_raw(sizeof(HeroUnwoundGroup));
    memset(g, 0, sizeof *g);
    g->landing = target;
    g->frames_n = walk.frames_n;
    memcpy(g->frames, walk.frames, sizeof(uintptr_t) * (size_t)walk.frames_n);
    hero_unwind_adopt(g, &walk);
    me->kind = kind;
    me->env = env;
    me->value = value;
    me->target = target;
    me->group = g;
    me->frame_tabled = false;
    me->frame_reached = false;
    me->missed = false;
    me->on = 1;
    __atomic_add_fetch(&hero_unwind_running, 1, __ATOMIC_RELAXED);
    memset(&me->exception, 0, sizeof me->exception);
    me->exception.exception_class = HERO_UNWIND_CLASS;
    /* UNDER ADDRESSSANITIZER THE SHADOW OF THE FRAMES THE UNWIND LEAVES IS
     * CLEARED FIRST, as ASan's own interceptor does before a C++ `throw`: the
     * unwinder steps on the stack this hook's frames stood on, and a redzone
     * of theirs left in the shadow stopped ASan itself in libunwind's
     * `__bzero` (`CHECK failed: asan_thread.cpp:369`, this lane's first
     * Darwin `--sanitize` run of its cases, 2026-11-27). */
    hero_unwind_clear();
    (void)_Unwind_ForcedUnwind(&me->exception, hero_unwind_stop, NULL);
    /* Back here only where the unwinder refused before it left any frame:
     * the jump is the caller's, as it always was. */
    hero_unwind_finish(me);
}

#elif !defined(_WIN32)

/* A machine whose frame pointer this file does not name: no unwind, the jump
 * as it always was. */
static void hero_unwind_jump(void *env, int value, int kind, uintptr_t target, uintptr_t lo, uintptr_t hi) {
    (void)env;
    (void)value;
    (void)kind;
    (void)target;
    (void)lo;
    (void)hi;
}

#endif

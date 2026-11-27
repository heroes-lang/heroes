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
 * a jump made inside a signal handler (no unwind is async-signal-safe), or
 * while the thread blocks any signal, or from a frame stopped anywhere but
 * just after a call; a jump made off the thread's own stack or over frame
 * records that do not rise on it as one chain (`hero_unwind_chained`); a
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
 * can falsify (panel 259), and by the number the nearest thunk at or above it
 * held at the jump, which every thunk's activation draws for itself: a C
 * library that copies coroutines' stacks in and out of one region stands two
 * activations at one address in turn, and the pointer alone took the one's
 * return for the other's (`hero_unwind_same`, step 214d). A group whose catch is abandoned in turn by a later
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
#include <signal.h>
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

/* A frame's activation, where no number tells it (`HeroUnwoundGroup`). */
#define HERO_UNWIND_BY_ADDRESS ((uintptr_t)0)
#define HERO_UNWIND_UNTOLD ((uintptr_t)1)

/* The values one unwind kept, and the frames whose return releases them.
 * Each frame is told by its pointer AND by the number the nearest thunk at or
 * above it held at the jump, read again where the frame returns (`slots`, `tokens`):
 * a C library that copies coroutines' stacks in and out of one region runs
 * two coroutines' frames at the same addresses in turn, and a frame pointer
 * alone took the one's return for the other's (below, `hero_unwind_same`).
 * A frame with no thunk above it, on a chain that ends whole, is on a stack
 * Heroes code started, where no other activation shares its address while it
 * stands, and is told by its pointer (`HERO_UNWIND_BY_ADDRESS`); one whose
 * chain the walk could not read to its end has no answer and is never taken
 * for returned (`HERO_UNWIND_UNTOLD`): its group waits for the program's end,
 * as every value did before R2. */
typedef struct HeroUnwoundGroup {
    struct HeroUnwoundGroup *next;
    HeroUnwoundValue *values;
    uintptr_t landing;
    int frames_n;
    uintptr_t frames[HERO_UNWIND_ABOVE];
    uintptr_t slots[HERO_UNWIND_ABOVE];
    uint64_t tokens[HERO_UNWIND_ABOVE];
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
    /* How many frames the walk met below the target, and how many the stop
     * has been asked about: the stop knows the target's frame by its stack
     * pointer and by its place in the walk, never by an order of addresses
     * (`hero_unwind_stop`). */
    int below;
    int seen;
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

/* THE THUNKS' PLACES: for each thunk's code, where its mark lies from its
 * frame pointer, the same in every activation of that code, so a walk that
 * meets a frame of a thunk's code (`_Unwind_GetRegionStart`) reads the number
 * its activation holds. Appended under the lock, read without it: an entry is
 * written before the count that publishes it. Past the table's end no thunk
 * is told, and `hero_unwind_places_full` makes every frame the walk cannot
 * place untold rather than told by its address. */
#define HERO_UNWIND_PLACES 1024
typedef struct {
    uintptr_t code;
    intptr_t at;
} HeroUnwindPlace;
static HeroUnwindPlace hero_unwind_places[HERO_UNWIND_PLACES];
static int64_t hero_unwind_places_n = 0;
static int64_t hero_unwind_places_full = 0;

/* This thread's side of a thunk's entry, one thread-local object so the
 * entry asks the thread's storage once (on Darwin each object read so is a
 * call): the code it placed last, and the block of numbers it draws from. */
typedef struct {
    uintptr_t placed;
    uint64_t next;
    uint64_t end;
} HeroUnwindEntering;
static _Thread_local HeroUnwindEntering hero_unwind_entering;

/* The place of the thunk whose code starts at `code`, or -1. */
static int64_t hero_unwind_place_of(uintptr_t code) {
    int64_t n = __atomic_load_n(&hero_unwind_places_n, __ATOMIC_ACQUIRE);
    for (int64_t i = 0; i < n; i += 1) {
        if (hero_unwind_places[i].code == code) return i;
    }
    return -1;
}

static void hero_unwind_place(HeroUnwindEntering *mine, uintptr_t code, intptr_t at) {
    mine->placed = code;
    if (hero_unwind_place_of(code) >= 0) return;
    hero_unwind_take();
    int64_t n = hero_unwind_places_n;
    if (hero_unwind_place_of(code) < 0) {
        if (n < HERO_UNWIND_PLACES) {
            hero_unwind_places[n].code = code;
            hero_unwind_places[n].at = at;
            __atomic_store_n(&hero_unwind_places_n, n + 1, __ATOMIC_RELEASE);
        } else {
            __atomic_store_n(&hero_unwind_places_full, 1, __ATOMIC_RELEASE);
        }
    }
    hero_unwind_drop();
}

/* The activations' numbers: never 0, never given twice in a run, each thread
 * taking them a block at a time. */
static uint64_t hero_unwind_numbers = 0;

/* A thunk of a program that unwinds was entered: its mark's initialiser. It
 * opens the program to the hook, places the thunk's mark once per thread and
 * code, and gives this activation its number, which the mark holds. */
uint64_t hero_unwind_handed(void *slot, void *frame, void (*self)(void)) {
    if (__atomic_load_n(&hero_unwind_scoped, __ATOMIC_RELAXED) == 0) {
        __atomic_store_n(&hero_unwind_scoped, 1, __ATOMIC_RELAXED);
    }
    HeroUnwindEntering *mine = &hero_unwind_entering;
    uintptr_t code = (uintptr_t)self;
    if (__builtin_expect(mine->placed != code, 0)) {
        hero_unwind_place(mine, code, (intptr_t)((uintptr_t)slot - (uintptr_t)frame));
    }
    if (__builtin_expect(mine->next == mine->end, 0)) {
        uint64_t block = (uint64_t)1 << 20;
        mine->next = __atomic_fetch_add(&hero_unwind_numbers, block, __ATOMIC_RELAXED) + 1;
        mine->end = mine->next + block;
    }
    uint64_t number = mine->next;
    mine->next = number + 1;
    return number;
}

/* Whether the activation the group's `i`th frame was is the one standing at
 * its address now: the nearest thunk at or above it still holds the number it held
 * at the jump, read by the kernel, since the address may now be another
 * stack's or none (`parts/rebind_jump.c`, `parts/sigjump_save.c`). */
static bool hero_unwind_same(const HeroUnwoundGroup *g, int i);

/* Whether `frame` is one of the group's, the same activation. */
static bool hero_unwind_names(const HeroUnwoundGroup *g, uintptr_t frame) {
    for (int i = 0; i < g->frames_n; i += 1) {
        if (g->frames[i] == frame && hero_unwind_same(g, i)) return true;
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
    /* The first frame pointer whose record the chain's check did not find
     * sound, or its last: the walk never steps from a frame standing there
     * (`hero_unwind_chained`). */
    uintptr_t limit;
    /* Whether the record at the limit ends the chain (its saved frame
     * pointer 0), and whether the walk ended there, every frame above the
     * target met. */
    bool whole;
    bool ended;
    bool found;
    bool passed;
    bool stopped;
    int left_n;
    uintptr_t left[HERO_UNWIND_LEFT];
    /* The frames at and above the target met, the first `HERO_UNWIND_ABOVE`
     * kept; and the thunks among them, by their place in that count, each
     * with its mark's address and the number it held (`HERO_UNWIND_UNTOLD`
     * where the kernel would not read it). */
    int above_n;
    int frames_n;
    uintptr_t frames[HERO_UNWIND_ABOVE];
    int thunks_n;
    int thunk_at[HERO_UNWIND_ABOVE + 1];
    uintptr_t thunk_slot[HERO_UNWIND_ABOVE + 1];
    uint64_t thunk_number[HERO_UNWIND_ABOVE + 1];
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
            if ((uintptr_t)_Unwind_GetGR(ctx, HERO_UNWIND_FP) >= walk->limit) {
                walk->stopped = true;
                return _URC_NORMAL_STOP;
            }
            return _URC_NO_REASON;
        }
    }
    uintptr_t fp = (uintptr_t)_Unwind_GetGR(ctx, HERO_UNWIND_FP);
    int at = walk->above_n;
    walk->above_n += 1;
    if (at < HERO_UNWIND_ABOVE) {
        walk->frames[at] = fp;
        walk->frames_n = at + 1;
    }
    /* A thunk at or above the frame the jump lands in: the number its
     * activation holds tells it, and the frames below it, from another
     * activation's at their addresses. A thunk is a frame of its own too:
     * at -O2 clang inlines the function a thunk calls, and the catch's call
     * with it, into the thunk (this lane's case of a copied coroutine's catch,
     * released early at -O2 alone while the thunk was asked only of the frames
     * below it). The first one past the kept frames is the last the walk
     * needs. */
    int64_t place = hero_unwind_place_of((uintptr_t)_Unwind_GetRegionStart(ctx));
    if (place >= 0 && walk->thunks_n < HERO_UNWIND_ABOVE + 1) {
        uintptr_t slot = fp + (uintptr_t)hero_unwind_places[place].at;
        uint64_t number = 0;
        int k = walk->thunks_n;
        walk->thunk_at[k] = at;
        walk->thunk_slot[k] = hero_unwind_peek(slot, &number, sizeof number) ? slot : HERO_UNWIND_UNTOLD;
        walk->thunk_number[k] = number;
        walk->thunks_n = k + 1;
        if (at >= HERO_UNWIND_ABOVE) return _URC_NORMAL_STOP;
    }
    /* The unwinder steps from this frame by reading its record, which the
     * chain's check read sound only below the limit. */
    if (fp >= walk->limit) {
        walk->ended = fp == walk->limit;
        return _URC_NORMAL_STOP;
    }
    return _URC_NO_REASON;
}

/* What tells each kept frame of the walk from another activation at its
 * address: the number of the nearest thunk at it or above it; with none, its address,
 * where the walk met every frame up to the chain's end and the thunks' table
 * held every thunk, and nothing otherwise. */
static void hero_unwind_told(HeroUnwoundGroup *g, const HeroUnwindWalk *walk) {
    bool whole = walk->whole && walk->ended && __atomic_load_n(&hero_unwind_places_full, __ATOMIC_ACQUIRE) == 0;
    for (int i = 0; i < g->frames_n; i += 1) {
        g->slots[i] = whole ? HERO_UNWIND_BY_ADDRESS : HERO_UNWIND_UNTOLD;
        g->tokens[i] = 0;
        for (int k = 0; k < walk->thunks_n; k += 1) {
            if (walk->thunk_at[k] >= i) {
                g->slots[i] = walk->thunk_slot[k];
                g->tokens[i] = walk->thunk_number[k];
                break;
            }
        }
    }
}

static bool hero_unwind_same(const HeroUnwoundGroup *g, int i) {
    uintptr_t slot = g->slots[i];
    if (slot == HERO_UNWIND_BY_ADDRESS) return true;
    if (slot == HERO_UNWIND_UNTOLD) return false;
    uint64_t now = 0;
    return hero_unwind_peek(slot, &now, sizeof now) && now == g->tokens[i];
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
    /* THE TARGET'S FRAME BY ITS IDENTITY: the one standing at the jump's stack
     * pointer exactly, or, should no frame stand there, the one at the walk's
     * place for it, so no cleanup of the frame the jump lands in or of any
     * above it ever runs. Never `CFA >= target`: frames on two stacks are not
     * ordered by their addresses, and where C's stack lies below a stack a
     * callback runs on, that order stopped an unwind at its first frame (panel
     * 263's compiler-engineer, `fu/fu.c`, 0 of 3 slots released). */
    me->seen += 1;
    if ((actions & _UA_END_OF_STACK) != 0 || (uintptr_t)_Unwind_GetCFA(ctx) == me->target || me->seen > me->below) {
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
        /* And the catch the group waits for is the one standing there now, not
         * another coroutine's at its address (`hero_unwind_same`): told by the
         * frame the jump landed in, the group's second, the first being the
         * one it abandoned at the target. */
        left = left && hero_unwind_same(g, g->frames_n > 1 ? 1 : 0);
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

/* WHETHER THE THREAD BLOCKS NO SIGNAL, asked of the kernel: a handler runs with
 * the signal it handles blocked unless it asked otherwise, so a jump made with
 * any signal blocked may come out of a handler that interrupted a frame in the
 * middle of its code, and is not unwound. This is the answer the frames could
 * not give: a handler a raw system call installed on Darwin returns through a
 * trampoline of the program's own, an ordinary C function whose return
 * address the walk reads as a call's, and `wdr` still released an array twice
 * in 11 runs of 34 with every walked frame standing after a call (this lane's
 * runs of the repair, 2026-11-27). A program that blocks signals of its own
 * accord keeps the behaviour before R2, the safe side. */
static bool hero_unwind_unmasked(void) {
    sigset_t now;
    if (pthread_sigmask(SIG_SETMASK, NULL, &now) != 0) return false;
    for (int sig = 1; sig < NSIG; sig += 1) {
        if (sigismember(&now, sig) == 1) return false;
    }
    return true;
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
 * same way, the safe side.
 *
 * AND HOW FAR ABOVE THE TARGET THE WALK MAY STEP: the walk goes on past the
 * target for the frames whose return releases the group, and the unwinder
 * reads each of their records too. So the records are read on, up to
 * `HERO_UNWIND_ABOVE` past the target, and the answer is the frame pointer of
 * the first whose record is not sound, or of the last read: the walk never
 * steps from a frame standing there. Until 2026-11-27 the check stopped at the
 * target, and a jump inside a coroutine whose frames a library copies in and
 * out of one region of the thread's stack walked on, past the landing, to the
 * coroutine's first frame, whose record its library writes zero: SIGSEGV at
 * address -8 inside `_Unwind_Backtrace` (`run/fixedbugs-a-c-longjmp-inside-
 * coroutines-copied-out-and-in-of-one-carved-region-keeps-their-large-values`,
 * `-O0`, 3 runs of 3, this lane). 0 where the chain below the target is not
 * one. */
static uintptr_t hero_unwind_chained(uintptr_t fp, uintptr_t target, uintptr_t lo, uintptr_t hi, bool *whole) {
    *whole = false;
    for (int steps = 0; steps < 100000; steps += 1) {
        bool readable = fp >= lo && fp + 2 * sizeof(uintptr_t) <= hi && (fp & (sizeof(uintptr_t) - 1)) == 0;
        uintptr_t next = readable ? ((const uintptr_t *)fp)[0] : 0;
        bool sound = readable && next > fp && next < hi;
        if (fp >= target) {
            if (!sound) {
                *whole = readable && next == 0;
                return fp;
            }
        } else if (!sound) {
            return 0;
        }
        fp = next;
    }
    return 0;
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
    if (me->on || !hero_unwind_unmasked()) return;
    bool whole = false;
    uintptr_t limit = hero_unwind_chained((uintptr_t)__builtin_frame_address(0), target, lo, hi, &whole);
    if (limit == 0) return;
    HeroUnwindWalk walk;
    memset(&walk, 0, sizeof walk);
    walk.target = target;
    walk.lo = lo;
    walk.hi = hi;
    walk.limit = limit;
    walk.whole = whole;
    _Unwind_Backtrace(hero_unwind_walked, &walk);
    if (walk.stopped || !walk.found || walk.frames_n == 0) return;
    HeroUnwoundGroup *g = (HeroUnwoundGroup *)hero_malloc_raw(sizeof(HeroUnwoundGroup));
    memset(g, 0, sizeof *g);
    g->landing = target;
    g->frames_n = walk.frames_n;
    memcpy(g->frames, walk.frames, sizeof(uintptr_t) * (size_t)walk.frames_n);
    hero_unwind_told(g, &walk);
    hero_unwind_adopt(g, &walk);
    me->kind = kind;
    me->env = env;
    me->value = value;
    me->target = target;
    me->below = walk.left_n;
    me->seen = 0;
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

#else

/* No walk here, so no group: every frame is its address. */
static bool hero_unwind_same(const HeroUnwoundGroup *g, int i) { return g->slots[i] == HERO_UNWIND_BY_ADDRESS; }

#endif

#if !defined(_WIN32) && !(defined(__aarch64__) || defined(__x86_64__))

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

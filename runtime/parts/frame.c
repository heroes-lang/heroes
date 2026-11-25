/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/frame.c — the room of a large value, off its function's frame (defect
 * 2278; panel 233, provisional, author ratification pending), and the one
 * bound it shares with the buffers lent to C (panel 259's R1, provisional,
 * author ratification pending; design.md §1.12).
 *
 * A value of a program's own type larger than the emitter's threshold
 * (`selfhost/emit/large_value.hero`, 4,096 bytes as it counts them) is not a
 * local of its function's C frame: the prologue takes its room here, the
 * function's one exit gives it back, and the body names it through a pointer.
 * So no frame holds a record of records megabytes deep, which an 8 MiB stack
 * could not (the other face of defect 2253's frame), and clang's SROA, which
 * splits only what a frame holds, never sees such a value whole (panel 233
 * measured 314 billion instructions and 12.3 GB at `-O2` for a 4 MiB record,
 * 0.44 billion and 12 MB with this room).
 *
 * EVERY TAKE IS A BLOCK OF ITS OWN, GIVEN BACK BY ITS OWN GIVE, in any order
 * and on any thread, and NOTHING HERE IS EVER JUDGED GIVEN BACK BY ITS FRAME'S
 * ADDRESS (panel 259's R1, route E). Until that sitting the room was a stack
 * of its own on each thread, and a C jump over a callback left takes on top
 * of it that nobody gave back, so panel 258's R1 healed it by address: a
 * take, a thunk's entry or a give first gave back every take of a frame below
 * its own on the thread's stack, and step 181a stood a coroutine's takes
 * aside where the runtime could name its stack. An order of addresses is a
 * fact a switch of stacks C makes can falsify, seen or not: a coroutine
 * library's own switch onto memory carved from the thread's stack (one
 * coroutine was enough), Boost.Context 1.92's own switch, and the designs that
 * copy a coroutine's frames out and back in (greenlet's on the thread's stack,
 * Tencent libco's on the heap, one over `makecontext` and `swapcontext` with
 * every switch seen) each put a live frame where the order said *gone*, and
 * the heal handed its room to another frame: 11 printed for a record holding
 * 7 on this Mac, SIGSEGV with nothing said on Linux arm64 and x86-64 (the
 * sitting's cells). A block of its own is right under every one of those
 * switches, since no other block's life is ever read from where its frame
 * stood.
 *
 * WHAT IT COSTS, said. A take C leaves without its give (a jump over a
 * callback, an exception through one, a coroutine nobody resumes) keeps its
 * block until the program ends, seen or not, since nothing here tells a frame
 * left for good from one suspended, and a loop of such leaving ends at the
 * bound below, where panel 258's heal gave the blocks back. And every take and
 * give goes through one lock of the process and the raw `malloc`: at `-O2`
 * about 1,932 instructions more a 32 KB take than the stack of chunks before it
 * (+32%, panel 259's synthesis, `cost_room`), a price of this list and not of
 * the route.
 *
 * The blocks come from the one `malloc` neither counter weighs and go back
 * through its mate (`hero_malloc_raw`, which stops at `out of memory` rather
 * than return NULL, and `hero_free_raw`, both in `parts/alloc.c` where Part
 * 7.13's single point puts every such call), so the leak gate counts what the
 * program owns and not this room. */

/* THE ROOM IS BOUNDED ONCE FOR THE WHOLE PROCESS, AND RUNNING OUT OF IT ABORTS
 * AS RUNNING OUT OF STACK DOES (spec § 9: *running out of stack aborts*).
 * Without a bound a recursion holding a large value would take the heap until
 * the system stopped the process, which on Linux is the OOM killer's SIGKILL
 * with nothing said. One gibibyte for every thread together, counted
 * atomically, a number and not a question asked of the machine, so a program
 * aborts at the same depth everywhere: panel 233's 128 MiB shapes held at most
 * 616 MB in it. Until the critic's second pass of that sitting it was a
 * gibibyte a THREAD, and three runaway threads held 3.29 GB before the first
 * bound fired.
 *
 * AND THE BUFFERS LENT TO C COUNT AGAINST THE SAME GIBIBYTE (panel 259's R1,
 * the coordinator's amendment): every region a buffer or a local is lent from
 * (`parts/lend.c`), by the bytes it maps, its guard page included, from its
 * lend to its give, live and left alike. A lend C leaves without its give is
 * held as a take is, and nothing else bounded those regions: 20,000 jumps over
 * a 4 KiB lend held 209 to 337 MB resident with no end in sight (`lendjump`,
 * the synthesis's run), which a longer loop ends where the system refuses a
 * mapping or, on Linux, by an out-of-memory kill with nothing said. A region
 * given back and kept for the next lend counts no longer. */
#define HERO_FRAME_ROOM ((size_t)1 << 30)

static _Atomic size_t hero_frame_held = 0;

/* THE MESSAGE OPENS AS THE STACK GUARD'S DOES AND SAYS WHICH ROOM RAN OUT:
 * `panic: stack exhausted in <module.function>`, the function whose prologue
 * asked or that lent, named as the guard names one where the platform's guard
 * names any (`parts/stack.c`, POSIX by `hero_stack_symbol` on the return
 * address, a generic instance's `static` copy included since defect 2371,
 * Windows the first sentence alone), then that the room large values take off
 * the stack is full, since the stack itself may be nearly empty. One thread
 * tells it, through the guard's own flag (`hero_stack_tell_first`, defect
 * 2287): a second that runs out while the first is telling, of its room or of
 * its stack, waits for the process to end, so two messages never interleave.
 */
static _Noreturn void hero_frame_exhausted(void *caller) {
    hero_report_begin();
#if !defined(_WIN32)
    hero_stack_tell_first();
    const char *who = caller != NULL ? hero_stack_symbol((uintptr_t)caller) : NULL;
    hero_stack_say("panic: stack exhausted");
    if (hero_stack_is_heroes(who)) {
        hero_stack_say(" in ");
        hero_stack_say_heroes_name(who);
    }
    hero_stack_say(": the room large values take off the stack is full, 1 GiB for the whole program\n");
    hero_abort();
#else
    (void)caller;
    hero_panic("stack exhausted: the room large values take off the stack is full, 1 GiB for the whole program");
#endif
}

/* `bytes` more held against the bound, or the abort, `caller` naming the
 * function that asked; and `bytes` given back. */
static void hero_frame_room_hold(size_t bytes, void *caller) {
    if (bytes > HERO_FRAME_ROOM) hero_frame_exhausted(caller);
    size_t before = atomic_fetch_add(&hero_frame_held, bytes);
    if (before > HERO_FRAME_ROOM - bytes) {
        atomic_fetch_sub(&hero_frame_held, bytes);
        hero_frame_exhausted(caller);
    }
}

static void hero_frame_room_free(size_t bytes) { atomic_fetch_sub(&hero_frame_held, bytes); }

/* THE LOCK OF THE BLOCKS AND OF THE LEND REGIONS' RECORDS. A block may be
 * given back on another thread than took it, a coroutine resumed there, and so
 * may a region (`parts/lend_aside.c`), so both are the process's, under one
 * lock each. */
#if defined(_WIN32)
typedef SRWLOCK hero_aside_mutex;
#define HERO_ASIDE_LOCK_INIT SRWLOCK_INIT
static void hero_aside_take(hero_aside_mutex *m) { AcquireSRWLockExclusive(m); }
static void hero_aside_drop(hero_aside_mutex *m) { ReleaseSRWLockExclusive(m); }
#else
typedef pthread_mutex_t hero_aside_mutex;
#define HERO_ASIDE_LOCK_INIT PTHREAD_MUTEX_INITIALIZER
static void hero_aside_take(hero_aside_mutex *m) { pthread_mutex_lock(m); }
static void hero_aside_drop(hero_aside_mutex *m) { pthread_mutex_unlock(m); }
#endif

/* One block's header. `newer` and `older` link every block standing, the
 * newest first, under the lock; `need` is what it holds against the bound. */
typedef struct HeroFrameBlock {
    struct HeroFrameBlock *newer;
    struct HeroFrameBlock *older;
    size_t need;
} HeroFrameBlock;

#define HERO_FRAME_HEADER ((sizeof(HeroFrameBlock) + 15) & ~(size_t)15)

static hero_aside_mutex hero_frame_lock = HERO_ASIDE_LOCK_INIT;
static HeroFrameBlock *hero_frame_newest = NULL;

static _Noreturn void hero_frame_unknown(void) {
    hero_panic("internal error: a large value's room was given back that this runtime never handed out, or "
               "handed back already (compiler bug)");
}

/* The function taking the room is named where the bound stops it, read from
 * where this was called, so this is never inlined. */
__attribute__((noinline)) void *hero_frame_take(size_t bytes) {
    void *caller = __builtin_return_address(0);
    if (bytes > HERO_FRAME_ROOM) hero_frame_exhausted(caller);
    size_t need = (bytes + 15) & ~(size_t)15;
    hero_frame_room_hold(need, caller);
    HeroFrameBlock *b = (HeroFrameBlock *)hero_malloc_raw(HERO_FRAME_HEADER + need);
    b->need = need;
    b->newer = NULL;
    hero_aside_take(&hero_frame_lock);
    b->older = hero_frame_newest;
    if (b->older != NULL) b->older->newer = b;
    hero_frame_newest = b;
    hero_aside_drop(&hero_frame_lock);
    return (char *)b + HERO_FRAME_HEADER;
}

/* The give of `p`: its block found in the list by the address it handed out
 * before a byte of the block is read, so a wrong pointer is the compiler's
 * error told and never a header read from where nothing was taken. The search
 * is as long as the blocks standing newer than `p`, every thread's, and a
 * function gives back at its exit what it took last, so the common case is the
 * first block it reads. */
void hero_frame_give(void *p) {
    hero_aside_take(&hero_frame_lock);
    HeroFrameBlock *b = hero_frame_newest;
    while (b != NULL && (char *)b + HERO_FRAME_HEADER != (char *)p) b = b->older;
    if (b == NULL) {
        hero_aside_drop(&hero_frame_lock);
        hero_frame_unknown();
    }
    if (b->newer != NULL) b->newer->older = b->older;
    else hero_frame_newest = b->older;
    if (b->older != NULL) b->older->newer = b->newer;
    hero_aside_drop(&hero_frame_lock);
    hero_frame_room_free(b->need);
    hero_free_raw(b);
}

/* The same, for `__attribute__((__cleanup__))`, which hands the address of
 * the pointer and runs after a `return` has copied its value: a function that
 * returns a group's record out of this room by C's convention gives its
 * regions back through here (`selfhost/emit/large_value.hero`'s
 * `taken_to_exit`, defect 2316). Given back before the `return`, the block
 * the value stood in would be freed before the return's copy read it. */
void hero_frame_give_at(void *slot) {
    hero_frame_give(*(void **)slot);
}

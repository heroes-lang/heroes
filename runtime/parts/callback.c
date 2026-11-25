/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/callback.c — the callbacks C is inside, counted, so that a frame a C
 * jump abandoned is told from a missing decref (defect 2704; design.md §1.12,
 * §4.19).
 *
 * THE FACT. A callback is a Heroes function C calls through its thunk
 * (`selfhost/emit/callback_thunk.hero`), and the thunk returns to C, or it does
 * not: C can `longjmp` over its frame (libpng's `png_error`, a watchdog whose
 * handler `siglongjmp`s out of the program's function), or switch away from it
 * for good. What the frames in between held, a `str`, an array, a map, is then
 * given back by nobody: the jump runs no code, the program's own end of scope
 * is a line of C the jump never reaches. That is C's jump and not a defect of
 * the compiler, and it is the same bargain a program makes by calling `exit`,
 * which `hero_exit` (`hero_os.h`) states: the values of the frames it leaves
 * are not audited.
 *
 * THE COUNT. Each thunk's first line enters (`hero_callback_entered`, which
 * calls `hero_callback_in`) and the line after its call returns (`hero_callback_left`):
 * the thread's count of thunks entered and not yet left. It is a plain thread-local
 * number, written by its own thread and read by it (in the end of `main`)
 * or by nobody (a thread's end adds what it still holds to one atomic sum), so
 * a callback costs two stores and a callback called a million times in a
 * sort costs no lock. A frame a jump abandoned never leaves, so the count
 * stays above zero for the rest of the thread: **at the end of the program a
 * count above zero says some callback's frame never finished**, abandoned by a
 * jump, or suspended on a coroutine nobody resumed, and either holds what it
 * held. The leak gate (`hero_runtime_check_leaks`, `parts/alloc.c`) then does
 * not read the heap-block balance as a missing decref: it cannot tell the
 * blocks those frames held from a leak of the compiler's, and saying *this is
 * a compiler bug* of a correct program was false (panel 257's ffi-pragmatist:
 * libpng's error path, 27 runs of 27).
 *
 * LEAKSANITIZER, WHERE IT STANDS (Linux). It is a leak detector and the blocks
 * those frames held are, to it, leaked: it would end the correct program with
 * its own report (exit 1 under `--sanitize`, 3 of 3 on Linux arm64) after the
 * gate above had rightly said nothing. Its documented hook for this,
 * `__lsan_is_turned_off`, which it asks before it checks, answers the same
 * question the gate answered: a program with a frame that never finished is
 * not audited for leaks, by either.
 *
 * WHAT THIS COSTS, said: a program in which a C jump abandoned a callback's
 * frame is not audited for a missing decref, as a program that calls `exit`
 * is not. Every other program is, as before.
 *
 * COUNTS CROSS THREADS ONLY AS A SUM. A callback entered on one thread may
 * return on another (a coroutine resumed on a different thread), so one
 * thread's count may be negative and another's positive: a thread's end adds
 * its own, and the gate reads the sum with the main thread's.
 *
 * AND THIS FILE IS WHERE THE RUNTIME LEARNS WHICH FRAMES A JUMP LEFT (panel
 * 258's R1, below): the gates' words for a count above zero, and the frame
 * each entry of the room and the lend regions was taken by. Needs
 * `parts/stack.c`'s `hero_stack_measure` and `parts/panic.c`. */

#if defined(_WIN32)
#include <windows.h>
#endif

/* The thread's own stack, measured once, and the ranges `makecontext` was
 * given that lie inside it, read again whenever one was given since
 * (`parts/stack_lane.c`, which reads and writes the rest). */
#define HERO_LANE_CARVED 4
typedef struct {
    uintptr_t lo;
    uintptr_t hi;
    int measured;
    unsigned seen;
    int carved;
    uintptr_t carved_lo[HERO_LANE_CARVED];
    uintptr_t carved_hi[HERO_LANE_CARVED];
} HeroStackOwn;

/* The thread's count of callbacks entered and not left, and beside it how
 * many entries of the large-value room and the lend regions the thread holds
 * (panel 258's R1, below), in one thread-local, so that a thunk's entry,
 * which writes the first, reads the second where the address is already in
 * hand: on Darwin each thread-local is reached through a call of its own,
 * and the healing's question at every thunk's entry, asked of the room's and
 * the regions' own counts, cost 46 instructions a callback at `-O2` and 65 at
 * `-O0` (20 million callbacks, instructions retired, `cost_cb`). `held` is a
 * hint and never a judgement: a count too high asks the room and the regions
 * for nothing, and every entry those find is judged by its frame. */
typedef struct {
    int64_t inside;
    int64_t held;
} HeroCallbackThread;

static _Thread_local HeroCallbackThread hero_callback_thread = {0, 0};
static _Thread_local HeroStackOwn hero_stack_own = {0};
static _Atomic int64_t hero_callbacks_unfinished_by_ended = 0;

static void hero_callback_in(void) { hero_callback_thread.inside += 1; }

void hero_callback_left(void) { hero_callback_thread.inside -= 1; }

/* An entry of the room or of the lend regions taken (`by` 1) or given back
 * (`by` -1, or minus all of them as a thread ends). */
static void hero_callback_held(int64_t by) { hero_callback_thread.held += by; }

/* A thread the program started ends: what it still holds joins the sum. */
static void hero_callback_thread_ends(void) {
    int64_t held = hero_callback_thread.inside;
    if (held != 0) atomic_fetch_add_explicit(&hero_callbacks_unfinished_by_ended, held, memory_order_relaxed);
    hero_callback_thread.inside = 0;
}

/* Callbacks entered and not left, at the end of the program: the main
 * thread's own, and every ended thread's. Positive says a frame never
 * finished. */
static int64_t hero_callbacks_unfinished(void) {
    return hero_callback_thread.inside + atomic_load_explicit(&hero_callbacks_unfinished_by_ended, memory_order_relaxed);
}

/* THE GATES' WORDS WHERE A CALLBACK NEVER RETURNED (panel 258's R1,
 * provisional, author ratification pending; defects 2753 and 2756). The
 * handle, lease and scratch gates of `hero_runtime_check_leaks`
 * (`parts/alloc.c`) keep the aborts spec § 13 promises (*"a lease nobody ends,
 * like a handle nobody consumes or a thread nobody waits for, aborts when
 * `main` returns, saying how many"*), and where this count is above zero they
 * say the cause they can see instead of *this program is missing that many*,
 * which accused a correct program of a release its source writes and C's jump
 * skipped (`handlejump`, 3 runs of 3 before this). Neither gate can tell which
 * of what it counts the abandoned frames held, so the words name both: the
 * count, and that a release those frames would have run never ran. */
#define HERO_CALLBACKS_LEFT                                                                                       \
    "%lld callback(s) of this program never returned: C jumped over their frames (`longjmp`, `siglongjmp`, a " \
    "signal handler's jump) or ended their thread, and "

/* WHICH FRAMES A C JUMP LEFT (panel 258's R1, provisional, author ratification
 * pending; defects 2752 and 2756). Two more stacks of this runtime are kept in
 * step with the program's frames, one each thread: the room a large value
 * takes off its function's frame (`parts/frame.c`) and the regions a buffer
 * or a local is lent to C from (`parts/lend.c`). The function that took an
 * entry gives it back at its one exit, so a jump over that function leaves the
 * entry standing with nothing to give it back: the room was then given back
 * out of order and said *compiler bug* (`room_order`, exit 134, 3 runs of 3),
 * and the regions climbed one depth a jump (`lendjump`, 334.7 MB resident
 * after 20,000 jumps against 3.0 MB where C returns).
 *
 * So each entry remembers the frame that took it, read by the take itself
 * from where it was called (HERO_CALLER_FRAME, below), and the two stacks
 * HEAL: where a function takes an entry, or a thunk is entered, every entry of
 * a frame deeper than the one asking, on this thread's stack, belongs to a
 * frame that is gone, since the one asking is the deepest alive; it is given
 * back first. A frame's mark is the same for its whole call and lower than
 * every caller's, wherever its function was inlined. The thunk's entry is what
 * catches a frame jumped over and then called again at the same address (the
 * loop of `lendjump`), which no take of the new frame can tell from its own.
 *
 * THE MARK IS THE CALLER'S FRAME, ASKED BY THE TAKE, so the program's C calls
 * the takes as it always has and the unit's headers gain no macro (panel 214's
 * R4 holds the runtime's macros to `HERO_`; a first form of this repair, the
 * three takes as macros handing in `__builtin_frame_address(0)`, broke it,
 * the compiler's own test told it). Each take is never inlined and reads:
 *
 *   - on POSIX, its caller's frame record, `__builtin_frame_address(1)`. Every
 *     unit keeps one in each function that calls another, the word
 *     `-fno-omit-frame-pointer` on x86-64 and AArch64's own default
 *     (`selfhost/cli/flags.hero`'s `frame_words`, which an abort's walk of
 *     its calls needs too, panel 253's R5), and a function that calls a take
 *     calls another;
 *   - on Windows x86-64, which keeps no frame record there, the address its
 *     return address was pushed at, `_AddressOfReturnAddress()`: the caller's
 *     stack pointer at the call, which the Windows x64 calling convention
 *     keeps still outside a function's prologue and epilogue for every
 *     function with no dynamic allocation, and the program's C allocates
 *     none on the stack. Unrun on Windows on the day it was written (the box
 *     was off; the CI's Windows leg is the first to run it).
 *
 * ON THIS THREAD'S OWN STACK, AND ONLY THERE. A coroutine runs on a stack of
 * its own, above or below this one, and its frames are no deeper than this
 * one's for being at lower addresses: an entry or a frame outside the thread's
 * own stack, measured once (Windows asks the thread's block, which a fiber's
 * switch changes), is never judged gone. Since the room and the regions hold
 * only what the thread's own stack took (`parts/stack_lane.c`, the defect
 * lane panel258-r1 found beside 2752),
 * an entry of a coroutine's stack stands aside, judged among its own stack's
 * entries alone (`parts/frame_aside.c`, `parts/lend_aside.c`).
 *
 * WHAT IT DOES NOT DO, said: nothing here releases a value a frame held (a
 * `str`, an array, a handle); those stay leaked, the heap-block audit forgiven
 * as above (panel 258's R2 is the release, pending a spec sentence). */

/* How many ranges `makecontext` was given in this process, counted after each
 * is written (`parts/sigentries.c`, `parts/rebind_entries.c`): a thread whose
 * count is behind reads again which of them lie inside its own stack. Windows
 * has no such range: a fiber's switch changes the thread's block instead. */
#if !defined(_WIN32)
static _Atomic unsigned hero_stacks_made = 0;
#endif

/* The mark of the frame that called the runtime function this is written in,
 * which must be `noinline` (the head's THE MARK IS THE CALLER'S FRAME). */
#if defined(_WIN32) && defined(__x86_64__)
#include <intrin.h>
#define HERO_CALLER_FRAME() ((uintptr_t)_AddressOfReturnAddress())
#else
#define HERO_CALLER_FRAME()                                                                              \
    (_Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wframe-address\"")(uintptr_t) \
         __builtin_frame_address(1) _Pragma("clang diagnostic pop"))
#endif

/* Whether `deeper` is the address of a frame below `frame`, both on this
 * thread's own stack. */
static bool hero_frame_is_below(uintptr_t deeper, uintptr_t frame) {
    if (deeper >= frame) return false;
#if defined(_WIN32)
    const NT_TIB *tib = (const NT_TIB *)NtCurrentTeb();
    uintptr_t lo = (uintptr_t)tib->StackLimit;
    uintptr_t hi = (uintptr_t)tib->StackBase;
#else
    HeroStackOwn *own = &hero_stack_own;
    if (!own->measured) {
        hero_stack_measure(&own->lo, &own->hi);
        own->measured = 1;
    }
    uintptr_t lo = own->lo;
    uintptr_t hi = own->hi;
#endif
    return deeper >= lo && frame < hi;
}

/* `parts/frame.c` and `parts/lend.c`, later in this unit: each gives back the
 * entries of frames below `frame`; and their asides, the entries of `stack`'s
 * frames below `frame` (`parts/frame_aside.c`, `parts/lend_aside.c`). */
static void hero_frame_heal_below(uintptr_t frame);
static void hero_lend_heal_below(uintptr_t frame);
static void hero_frame_aside_heal(uintptr_t stack, uintptr_t frame);
static void hero_lend_aside_heal(uintptr_t stack, uintptr_t frame);

/* `parts/stack_lane.c`: whether `at` is on the thread's own stack, and where
 * it is not, the stack it is on (0 for one nobody can name). */
static inline __attribute__((always_inline)) int hero_stack_lane(uintptr_t at, uintptr_t *stack);

/* A thunk is entered at `frame`: nothing of this thread's stack below it is
 * alive (`sigguard.c`'s `hero_callback_entered`, after its question), and
 * nothing AT it either, since this is the thunk's first line and it has taken
 * nothing yet. The second half is what catches a callback inlined into its
 * thunk, as clang does at `-O2`: its takes name the thunk's own frame, and a
 * jump over it and the next call of the thunk at the same address left them
 * standing at exactly that frame (`lendjump` at `-O2`, 334.7 MB resident, run,
 * while this line said *below*). So every entry at or below `frame`. It rests
 * on the thunk being a frame of its own, which `emit/callback_thunk.hero`'s
 * `OWN_FRAME` makes it: a thunk inlined into the function a header's inline C
 * was inlined into named that function's frame, and this line then gave back
 * the room that function still held (defect 2286's case at `-O2`, run). A
 * thunk entered on a coroutine's stack judges that stack's entries alone, and
 * one on a stack nobody can name judges nothing (`parts/stack_lane.c`). */
static void hero_callback_heal(void *frame) {
    if (hero_callback_thread.held == 0) return;
    uintptr_t above = (uintptr_t)frame + 1;
    uintptr_t stack = 0;
    if (hero_stack_lane((uintptr_t)frame, &stack)) {
        hero_frame_heal_below(above);
        hero_lend_heal_below(above);
    } else if (stack != 0) {
        hero_frame_aside_heal(stack, above);
        hero_lend_aside_heal(stack, above);
    }
}

#if defined(__linux__) && defined(__has_feature)
#if __has_feature(address_sanitizer)
/* LeakSanitizer's own hook (`sanitizer/lsan_interface.h`), a function the
 * program may define: nonzero turns its check at the end of the process off. */
int __lsan_is_turned_off(void);
int __lsan_is_turned_off(void) { return hero_callbacks_unfinished() > 0; }
#endif
#endif

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
 * the thread's count of thunks entered and not yet left. A frame a jump
 * abandoned never leaves, so the count stays above zero for the rest of the
 * thread: **at the end of the program a count above zero says some callback's
 * frame never finished**, abandoned by a jump, or suspended on a coroutine
 * nobody resumed, and either holds what it held. The leak gate
 * (`hero_runtime_check_leaks`, `parts/alloc.c`) then does not read the
 * heap-block balance as a missing decref: it cannot tell the blocks those
 * frames held from a leak of the compiler's, and saying *this is a compiler
 * bug* of a correct program was false (panel 257's ffi-pragmatist: libpng's
 * error path, 27 runs of 27).
 *
 * EVERY THREAD'S COUNT IS THE PROCESS'S, READ WHILE THE THREAD RUNS (defect
 * 2854). Until 2026-11-26 each count was a thread-local that
 * joined one atomic sum only when its thread ended, so the room's abort
 * (`parts/frame.c`), which tells the count while other threads of the program
 * still run, said the calling thread's alone: two threads jumping over
 * callbacks that hold room filled the gibibyte together and the abort said
 * 829, 1,286, 1,198 or 919 callbacks had not returned (four runs, this Mac),
 * where the two had left about 2,047 between them. So the counts live here,
 * one atomic per thread that may run a callback: `[0]` the thread that runs
 * `main`, `[1 + i]` the thread in the spawn table's slot `i` (`parts/spawn.c`),
 * the only threads a callback runs on (`parts/thread.c`'s guard). Each is
 * written by one thread at a time, its own, with a relaxed load and a relaxed
 * store, which the three targets compile to the plain load and store a
 * thread-local cost (a read-modify-write is never needed: no other thread
 * writes it), so a callback called a million times in a sort still costs no
 * lock and no locked instruction. A thread's count is never moved at its end:
 * it stays in its slot, and the next thread the slot starts goes on from it,
 * ordered after it by the join that freed the slot, so the sum of the table
 * is every thread's, ended or running, and nothing is ever read twice or
 * missed in a move. A thread C ended (`pthread_exit`, Windows' `ExitThread`,
 * which runs no handler of this runtime's) keeps its count with it, which the
 * end of a thread used to drop on Windows.
 *
 * THE READ TAKES NO LOCK, so it cannot deadlock under any lock the reader
 * holds: the room's abort runs inside the lend regions' lock where a lend
 * takes room (`parts/lend_aside.c`, `hero_frame_room_hold` under
 * `hero_lend_aside_lock`), and a read that waited there for the spawn table's
 * lock, or a slot's, could wait on a thread that waits for the lend's.
 * It is 257 lock-free atomic loads (asserted below), each a value its count
 * held at some moment of the read, never a torn one, and exact for a thread
 * whose last callback the program's own synchronisation orders before the
 * read (a `wait`, a mailbox, a lock); a thread still entering callbacks while
 * the abort reads is counted as it stood at that load. A thread the program
 * did not start has no count (`hero_callback_mine` NULL): it is refused by
 * name at the entry of the function C called (`hero_thread_guard`) before
 * that function runs, so the entry and the leave it would count never
 * frame a body.
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
 * thread's count may be negative and another's positive: every reader reads
 * the sum of the table.
 *
 * AND THE COUNT IS TOLD WHERE THE PROGRAM STOPS FOR WHAT THOSE FRAMES HELD:
 * by the gates' words (panel 258's R1, below), and by the room's abort, which
 * says how many callbacks have not returned (panel 259's R2,
 * `parts/frame.c`). Needs `parts/panic.c`. */

#if defined(_WIN32)
#include <windows.h>
#endif

/* One count for the thread that runs `main` and one for each slot of the
 * spawn table, `HERO_SPAWN_SLOTS` (`parts/spawn.c`, which asserts the two
 * agree). */
#define HERO_CALLBACK_MAIN 0
#define HERO_CALLBACK_THREADS (1 + 256)

_Static_assert(__atomic_always_lock_free(sizeof(int64_t), 0),
               "a callback count is read by another thread without a lock, so it must be a lock-free atomic");

/* Callbacks entered and not left, by the thread that entered or left them. */
static _Atomic int64_t hero_callback_counts[HERO_CALLBACK_THREADS];

/* This thread's count in the table; NULL on a thread the program did not
 * start, whose callback is refused before its body runs. */
static _Thread_local _Atomic int64_t *hero_callback_mine = NULL;

/* The thread claims its count, `at` in the table, before any Heroes code runs
 * on it (`hero_thread_claim`, `parts/thread.c`). */
static void hero_callback_claim(size_t at) { hero_callback_mine = &hero_callback_counts[at]; }

/* One more, or one less, on this thread's count: its own thread is the only
 * writer, so a load and a store, both relaxed, are the whole of it. */
static void hero_callback_step(int64_t by) {
    _Atomic int64_t *mine = hero_callback_mine;
    if (mine == NULL) return;
    atomic_store_explicit(mine, atomic_load_explicit(mine, memory_order_relaxed) + by, memory_order_relaxed);
}

static void hero_callback_in(void) { hero_callback_step(1); }

void hero_callback_left(void) { hero_callback_step(-1); }

/* Callbacks entered and not left in the whole program, every thread's, ended
 * or running, read without a lock (above). Positive says a frame never
 * finished. */
static int64_t hero_callbacks_unfinished(void) {
    int64_t sum = 0;
    for (size_t at = 0; at < HERO_CALLBACK_THREADS; at += 1) {
        sum += atomic_load_explicit(&hero_callback_counts[at], memory_order_relaxed);
    }
    return sum;
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

#if defined(__linux__) && defined(__has_feature)
#if __has_feature(address_sanitizer)
/* LeakSanitizer's own hook (`sanitizer/lsan_interface.h`), a function the
 * program may define: nonzero turns its check at the end of the process off. */
int __lsan_is_turned_off(void);
int __lsan_is_turned_off(void) { return hero_callbacks_unfinished() > 0; }
#endif
#endif

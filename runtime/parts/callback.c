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
 * WHAT THIS COSTS, said: a program in which a C jump abandoned a callback's
 * frame is not audited for a missing decref, as a program that calls `exit`
 * is not. Every other program is, as before.
 *
 * COUNTS CROSS THREADS ONLY AS A SUM. A callback entered on one thread may
 * return on another (a coroutine resumed on a different thread), so one
 * thread's count may be negative and another's positive: a thread's end adds
 * its own, and the gate reads the sum with the main thread's. */

static _Thread_local int64_t hero_callbacks_inside = 0;
static _Atomic int64_t hero_callbacks_unfinished_by_ended = 0;

static void hero_callback_in(void) { hero_callbacks_inside += 1; }

void hero_callback_left(void) { hero_callbacks_inside -= 1; }

/* A thread the program started ends: what it still holds joins the sum. */
static void hero_callback_thread_ends(void) {
    int64_t held = hero_callbacks_inside;
    if (held != 0) atomic_fetch_add_explicit(&hero_callbacks_unfinished_by_ended, held, memory_order_relaxed);
    hero_callbacks_inside = 0;
}

/* Callbacks entered and not left, at the end of the program: the main
 * thread's own, and every ended thread's. Positive says a frame never
 * finished. */
static int64_t hero_callbacks_unfinished(void) {
    return hero_callbacks_inside + atomic_load_explicit(&hero_callbacks_unfinished_by_ended, memory_order_relaxed);
}

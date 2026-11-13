/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/thread.c — the thread a Heroes program was started on (panel 111 R9).
 *
 * WHAT THIS IS FOR. Since panel 111 R4 a Heroes function may be handed to a C
 * function as a callback, and C is then free to call it back FROM A THREAD IT
 * MADE ITSELF — an audio callback, a signal handler's worker, a library's own
 * pool. Everything a Heroes value does on the way through is written for one
 * thread. Two of the three reasons this file gives below have since been
 * repaired and the sentence is corrected rather than left standing, because a
 * guard whose stated reason has expired is a guard somebody will remove:
 * `hero_str_incref` was a plain `int64_t` until M-isolated-threads step 3 and is
 * an `_Atomic` counter now, and `array.c`'s comparison scratch was process-wide
 * until step 4 and is `_Thread_local` now. **What is NOT repaired is the one in
 * the middle, and it is enough on its own**: `cow.c` decides in-place mutation
 * with `if (a->refcount == 1)` — a test and then a mutate, which no memory order
 * closes. Panel 111 built the
 * programs: a shared `str` across 32 threads is `heap-use-after-free` or a
 * double free in 9 ASan runs of 10, with the refcount drifting to 6290-9785
 * instead of 1, and `a == b` on nested arrays is exit 139 with an empty stderr.
 *
 * SO THE ANSWER IS NOT TO SURVIVE IT, IT IS TO REFUSE IT BY NAME. design.md
 * §1.12 asks that a Heroes program not segfault and not corrupt memory. A
 * program that stops and says which function C called is that promise KEPT, not
 * an exception to it — and it is what CPython and OCaml have both required for
 * thirty years, in CPython's case outliving the removal of the global lock.
 *
 * WHY A THREAD-LOCAL AND NOT `pthread_self()`. Measured, four runtimes built and
 * raced on the compiler's own test suite (docs/measurements/018): the same check
 * costs +6.6% through a `_Thread_local` and +19% through `pthread_self()`. The
 * brief predicted the opposite, by a factor of three, and only building both
 * revealed it. Those numbers are for the check placed at every str, array and
 * map operation; here it sits at the entry of the emitted callback alone, which
 * is where the danger actually enters, since C can reach Heroes code only
 * through an address Heroes handed it.
 *
 * WHAT A PROGRAM WITH NO CALLBACK PAYS: nothing at all. The emitter writes no
 * call to `hero_thread_guard` into a program that hands C no function, and
 * `hero_thread_claim` rides `hero_args_set` the way panel 104's stack guard
 * does, so the generated `main` is untouched.
 *
 * WHAT IS CLOSED, AND IT IS THIS GUARD THAT CLOSED IT — corrected 2026-09-06 by
 * panel 115's ffi seat, which measured both halves rather than reading this
 * paragraph. It used to say that a Heroes function which allocates nothing and
 * only recurses "still dies at exit 132 with an empty stderr. That is
 * `M-thread-stacks`' half by name (panel 107)." Both clauses were wrong the
 * moment M-isolated-threads shipped this file.
 *
 * The seat built the counterfactual: it took the emitted C of a program whose
 * callback recurses, DELETED the one line `hero_thread_guard(...)`, and rebuilt
 * against the M-thread-stacks runtime. **Exit 132, empty stderr.** So the
 * per-thread stack guard does nothing whatever for a foreign thread —
 * `hero_stack_guard_enter()` has exactly two callers, `hero_args_set` and
 * `hero_spawn_enter`, and a library's own thread reaches neither. What stops the
 * program is the line below, at its first instruction, before one frame of
 * recursion.
 *
 * Why the correction matters more than the fact: a reader who later narrows
 * `emit/callback_guard.hero`'s set would reopen a silent exit 132 while this
 * comment told them the hole belonged to another milestone that had closed.
 */

/* Zero on every thread the OS creates — C11 7.5: a thread-local object without
 * an initialiser starts at zero in each thread — and true on exactly the thread
 * that ran `main`. The whole test is therefore "did anybody claim this thread",
 * which needs no thread identifier and no comparison against one. */
static _Thread_local bool hero_thread_is_home = false;

/* `static`, like panel 104's `hero_stack_guard_install`: nothing outside this
 * runtime claims a thread, and the one caller is `hero_args_set` below in the
 * same translation unit. Only the guard itself is in the header, because only
 * the guard is called from generated C.
 *
 * And the thread's own stack is read here, once, for the question every thunk
 * asks first (`parts/sigguard.c`, panel 247's R1): a claim is made before any
 * Heroes code runs on the thread and, on a worker, while its signals are still
 * held off (`parts/spawn.c`, defect 2496), so the reading is never made inside
 * a signal, where glibc's `pthread_getattr_np` would allocate. */
static void hero_thread_claim(void) {
    hero_thread_is_home = true;
    hero_signal_measure();
}

/* The entry of every function this program may hand to C (panel 111 R4's
 * permission is what decides the set, and emit/callback_guard.hero computes
 * it). `what` is the Heroes name — `module.function`, already un-mangled by the
 * compiler that wrote the call — because the reader of this message is looking
 * at a .hero file and never at the C.
 *
 * THE WORDS NAME THE TWO DOORS, NOT A FILE (defect 2492; design.md §4.17, a
 * message carries what is needed to fix the mistake it names without opening
 * another file). Until 2026-11-13 the refusal cited `design.md Part 7.13`,
 * which a program's reader does not have, and both answers of panel 246's
 * blind reading for a first thread task met it with no word on what to write
 * instead, each having wrapped `pthread_create`. The two ways here are work
 * handed to C to run beside `main`, whose door is `hero_thread_spawn`, and a
 * function given to `signal`, which lands here on POSIX whenever a thread C
 * made takes the signal (panel 246's `whichthread`, run) and on Windows for
 * every Ctrl-C, whose handler the system runs on a thread of its own
 * (Microsoft's `signal` and HandlerRoutine pages, read by panel 246's
 * ffi-pragmatist, not run on the box); panel 246's R4 rules that no Heroes
 * code runs in a handler, and the stop a server needs is the platform
 * layer's. One line, so a case pins it whole.
 *
 * AND THEY ARE SAID AS EVERY REFUSAL OF A SIGNAL IS, ASYNC-SIGNAL-SAFE
 * (defect 2494, lane b35-signals): a handler of C's on a thread C made runs
 * this refusal inside the signal, and it formatted its words with `snprintf`
 * and printed them with stdio, so a signal landing in that thread's own
 * `snprintf` was killed by Darwin's locale lock with nothing said, 1 storm of
 * 40 on this Mac, 2026-11-13 (`_os_unfair_lock_recursive_abort < localeconv_l <
 * __snprintf_chk < hero_thread_guard < ... < _sigtramp < localeconv_l <
 * __snprintf_chk`). So the name is copied into fixed parts and the line goes
 * through `parts/sigguard.c`'s stop: `write(2)`, no stdio, SIGABRT raised,
 * latched once per process. The thunk C is handed asks for this refusal itself
 * on such a thread before any thread-local of this runtime is touched there;
 * the guard below is reached by a Heroes function's own entry. What is given
 * up: stdout is not flushed first, which `hero_panic` did and which may wait
 * for ever on a lock the signal interrupted; it is line-buffered on POSIX and
 * unbuffered on Windows (`parts/os.c`), so what a program printed whole is
 * already out, and a line it had not ended is lost. */
static _Noreturn void hero_thread_refuse(const char *what) {
    const char *const said[] = {
        "panic: ",
        what,
        " ran on a thread this program did not start: C called it back from a thread of its own, where this "
        "runtime's values are not safe. To run work on another thread, start it with `hero_thread_spawn` from "
        "`hero_os.h`, whose function runs on a thread this program owns. To stop on Ctrl-C or SIGTERM, give C "
        "no Heroes function as a signal handler: the platform layer's waiting calls, in `hero_net.h`, end with "
        "a stop the program reads",
    };
    hero_signal_stop(said, sizeof said / sizeof said[0]);
}

void hero_thread_guard(const char *what) {
    if (hero_thread_is_home) return;
    hero_thread_refuse(what);
}

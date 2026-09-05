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
 * thread: `hero_str_incref` is a plain `int64_t`, `cow.c` decides in-place
 * mutation with `if (a->refcount == 1)` — a test and then a mutate — and the
 * comparison scratch buffers in `array.c` are process-wide. Panel 111 built the
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
 * WHAT IS STILL OPEN, and it is named rather than hidden: a Heroes function
 * that allocates nothing and only recurses never reaches a guarded point on the
 * foreign thread's small stack and still dies at exit 132 with an empty stderr.
 * That is `M-thread-stacks`' half by name (panel 107), not this file's.
 */

/* Zero on every thread the OS creates — C11 7.5: a thread-local object without
 * an initialiser starts at zero in each thread — and true on exactly the thread
 * that ran `main`. The whole test is therefore "did anybody claim this thread",
 * which needs no thread identifier and no comparison against one. */
static _Thread_local bool hero_thread_is_home = false;

/* `static`, like panel 104's `hero_stack_guard_install`: nothing outside this
 * runtime claims a thread, and the one caller is `hero_args_set` below in the
 * same translation unit. Only the guard itself is in the header, because only
 * the guard is called from generated C. */
static void hero_thread_claim(void) { hero_thread_is_home = true; }

/* The entry of every function this program may hand to C (panel 111 R4's
 * permission is what decides the set, and emit/callback_guard.hero computes
 * it). `what` is the Heroes name — `module.function`, already un-mangled by the
 * compiler that wrote the call — because the reader of this message is looking
 * at a .hero file and never at the C. */
void hero_thread_guard(const char *what) {
    if (hero_thread_is_home) return;
    char msg[256];
    snprintf(
        msg,
        sizeof msg,
        "%s ran on a thread this program did not start — C called it back from a "
        "thread of its own, and this runtime's values are not safe there yet "
        "(design.md Part 7.13)",
        what
    );
    hero_panic(msg);
}

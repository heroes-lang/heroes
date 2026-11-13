/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigguard.c — no Heroes code runs inside a signal handler (panel 246's
 * R4, enforced by panel 247's R1, R2 and R9; defect 2494).
 *
 * WHAT IT IS FOR. A Heroes function handed to C may be called by C from inside
 * a signal: given to `signal` or to the program's own `sigaction`, or called by
 * a handler written in C. There it reaches C the signal may have interrupted
 * half way: this runtime's allocator (`parts/alloc.c` brackets it, panel 247's
 * D), the buffers lent to C (`parts/lend.c`), `snprintf` in every number
 * formatted, stdio, `exit`. On Darwin the C library then kills the process by
 * SIGKILL with nothing said (a recursive `os_unfair_lock` in `snprintf`'s
 * locale, 10 runs of 10 of panel 247's `clocale` and `wraplocale`); on Linux
 * the same program goes on, its stdio corrupted in silence (the sitting's
 * ffi-pragmatist, 50 million lines where 32 stood).
 *
 * WHERE THE QUESTION IS ASKED: the first line of every THUNK, the one door from
 * C into Heroes code (`selfhost/emit/callback_thunk.hero`), so a Heroes call by
 * name or through a value never asks it, and a program that hands C no
 * function has no thunk and no probe. Three questions, the cheapest first:
 *
 *   1. THE EXACT TEST. Is the thunk's own return address the delivery's? A
 *      signal's delivery calls a handler from one place in the system, Darwin's
 *      `_sigtramp`, Linux's `__kernel_rt_sigreturn` or glibc's `__restore_rt`,
 *      the same for a plain and an `SA_SIGINFO` handler, on any stack (315,846
 *      deliveries on Darwin arm64 and 2,001,938 on Linux arm64, one address
 *      each, never a call that was not a delivery: panel 247's ffi-pragmatist).
 *      It is learned once, below, and it needs no frame record and no list of
 *      the functions that install a handler: `signal`, `sigset`, `bsd_signal`,
 *      `sigvec`, a library's or the program's own `sigaction` alike.
 *   2. ONSTACK. Where the thunk's frame lies outside its thread's own stack,
 *      is the thread running on an alternate signal stack? Only a delivery
 *      puts it there (POSIX's `sigaltstack`, `SS_ONSTACK`), so a handler of
 *      C's on an alternate stack the program made, which no walk can climb out
 *      of, is answered by the kernel exactly (panel 247's `fown`).
 *   3. THE WALK. Was one of the HERO_SIGNAL_WALK frame records above the thunk
 *      entered by the delivery: a handler of C's that called the Heroes
 *      function, directly or through `exit`'s `atexit` list, `fork`'s
 *      `pthread_atfork` list or `qsort`? Bounded by the thread's own stack,
 *      read once per thread as it is claimed, never inside a signal.
 *
 * WHAT IT DOES NOT SEE, said, and `parts/alloc.c`'s guard (D) stays beneath it
 * for that (panel 247's R2 and R3): a handler whose frames keep no record (a
 * library built `-fomit-frame-pointer`), one more than HERO_SIGNAL_WALK frames
 * below the Heroes function (glibc's `qsort` of 10,000 values), a thread C
 * made (`parts/thread.c`'s guard refuses every Heroes function there), and
 * Windows, where a console's Ctrl-C runs on a thread Windows makes, which that
 * guard refuses, and a fault's handler runs on the faulting thread.
 *
 * THE ONE STOP. Every refusal of a re-entrance by a signal, this file's, the
 * allocator's and the lend tracking's, ends through `hero_signal_stop`, which
 * is async-signal-safe from its first call to its last (spec § 6, *"An abort
 * ends the program at once, saying why"*): its words assembled from fixed parts
 * and a name copied byte by byte, one `write(2)`, no `snprintf`, no stdio, no
 * `abort()`; SIGABRT's default put back, raised, `_exit(134)` if that returns.
 * It is LATCHED ONCE PER PROCESS: two threads inside a refusal at once wrote
 * their words over each other (panel 247's critic, pid 74286), so the second
 * writes nothing and waits for the end the first is making. And every refusal
 * carries the one remedy (panel 247's R9), naming no C header: a header two
 * modules include holds two of its `static` flags, and the second module's
 * handler is then never read (panel 247's spec-warden, `swp6`). */

#include <signal.h>
#if defined(_WIN32)
#include <io.h>
#include <windows.h>
#else
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#endif

/* What to write instead, the last words of every refusal below: spec § 13's
 * own sentence (panel 247's R5), each door named with its header and its
 * Heroes signature, as panel 247's R9 asks, and no C header, the trap a
 * `static` flag in one sets for a second module. */
#define HERO_SIGNAL_REMEDY                                                                                   \
    "No function of the program is a signal handler, nor called by one: after `hero_net_catch_stop() -> "  \
    "i64` (`hero_net.h`), Ctrl-C and SIGTERM only make `hero_net_stop_asked() -> i64` answer 1 and a "      \
    "waiting call of that layer answer stopped; `hero_signal_watch(sig: i32) -> i64` (`hero_os.h`) counts " \
    "any other signal, and `hero_signal_take(sig: i32) -> i64` gives the count since its last call"

/* 0 until the first refusal takes the stop. */
static _Atomic int hero_signal_stopping = 0;

/* Every byte of `text` to stderr by `write(2)`, `_write` on Windows. */
static void hero_signal_say(const char *text) {
    size_t left = 0;
    while (text[left] != '\0') left++;
    while (left > 0) {
#if defined(_WIN32)
        int put = _write(2, text, (unsigned int)left);
#else
        ssize_t put = write(2, text, left);
        if (put < 0 && errno == EINTR) continue;
#endif
        if (put <= 0) return;
        text += put;
        left -= (size_t)put;
    }
}

/* The stop. `parts` are written one after another, then a line feed: one
 * line, since the latch admits one writer, and no buffer, so a refusal made
 * on a small alternate signal stack the program gave takes none of it.
 *
 * Every signal is held off the calling thread before the latch is taken, so a
 * second signal cannot run a second refusal on the thread already making the
 * first, which would wait on the latch for ever; SIGABRT alone is let through
 * again to end the process. */
static _Noreturn void hero_signal_stop(const char *const *parts, size_t count) {
#if !defined(_WIN32)
    sigset_t all;
    sigfillset(&all);
    pthread_sigmask(SIG_BLOCK, &all, NULL);
#endif
    if (atomic_exchange_explicit(&hero_signal_stopping, 1, memory_order_acq_rel) != 0) {
        for (;;) {
#if defined(_WIN32)
            Sleep(INFINITE);
#else
            pause();
#endif
        }
    }
    hero_runtime_spoke = 1;
    for (size_t i = 0; i < count; i++) hero_signal_say(parts[i]);
    hero_signal_say("\n");
    signal(SIGABRT, SIG_DFL);
#if !defined(_WIN32)
    sigset_t abort_only;
    sigemptyset(&abort_only);
    sigaddset(&abort_only, SIGABRT);
    pthread_sigmask(SIG_UNBLOCK, &abort_only, NULL);
#endif
    raise(SIGABRT);
    _exit(134);
}

#if !defined(_WIN32)

/* How many frame records above the thunk the walk reads. Eight reaches a
 * handler calling the function directly (one), through `exit` (three on both
 * POSIX legs), `fork` (three or four) and `qsort` of a few values (two to
 * five); glibc's `qsort` of many is deeper, and D is what stands there. */
#define HERO_SIGNAL_WALK 8

/* The delivery's return address, for a plain handler and an `SA_SIGINFO` one,
 * 0 until learned; and whether it is: 0 not yet, 1 being learned, 2 learned. */
static _Atomic uintptr_t hero_signal_returns[2];
static _Atomic int hero_signal_state = 0;

static void hero_signal_probe_plain(int sig) {
    (void)sig;
    atomic_store_explicit(&hero_signal_returns[0], (uintptr_t)__builtin_return_address(0), memory_order_relaxed);
}

static void hero_signal_probe_info(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)info;
    (void)context;
    atomic_store_explicit(&hero_signal_returns[1], (uintptr_t)__builtin_return_address(0), memory_order_relaxed);
}

/* One probe: SIGURG to `probe` through the C library's own `sigaction`, never
 * through a definition of this runtime's (a runtime that defined it learned
 * its own wrapper's address and refused nothing: panel 247's critic), and the
 * action the program had put back. A thread-directed `raise` is delivered
 * before it returns. */
static void hero_signal_probe(struct sigaction *probe) {
    struct sigaction before;
    sigemptyset(&probe->sa_mask);
    if (sigaction(SIGURG, probe, &before) != 0) return;
    raise(SIGURG);
    sigaction(SIGURG, &before, NULL);
}

/* LEARNED ONCE PER PROCESS, at the first crossing of a function into C (the
 * emitted `hero_callback_of`), on the thread making it, with SIGURG let
 * through there for the two probes and its mask put back: a program that hands
 * C no function never raises it. Before that crossing no thunk's address is
 * anywhere C can call it. SIGURG is ignored by default and passed on without a
 * stop by lldb (panel 247's compiler-engineer); one sent from outside during
 * the two probes is taken by them, and a program's own handler and an ignored
 * SIGURG stay as they were (the ffi-pragmatist's `urg.c`, 100 of 100). */
void hero_signal_learn(void) {
    if (atomic_load_explicit(&hero_signal_state, memory_order_acquire) == 2) return;
    int expect = 0;
    if (!atomic_compare_exchange_strong(&hero_signal_state, &expect, 1)) {
        while (atomic_load_explicit(&hero_signal_state, memory_order_acquire) != 2) sched_yield();
        return;
    }
    sigset_t urg;
    sigset_t was;
    sigemptyset(&urg);
    sigaddset(&urg, SIGURG);
    pthread_sigmask(SIG_UNBLOCK, &urg, &was);
    struct sigaction probe;
    memset(&probe, 0, sizeof probe);
    probe.sa_handler = hero_signal_probe_plain;
    hero_signal_probe(&probe);
    memset(&probe, 0, sizeof probe);
    probe.sa_sigaction = hero_signal_probe_info;
    probe.sa_flags = SA_SIGINFO;
    hero_signal_probe(&probe);
    pthread_sigmask(SIG_SETMASK, &was, NULL);
    /* A probe the system refused learns nothing, and its kind is asked by the
     * other's address, the same on every leg measured; two refused leave the
     * questions below unasked rather than matching a frame whose return is 0.
     * The plain one is published last, with a release, so a thunk on any
     * thread that reads it with an acquire reads both. */
    uintptr_t plain = atomic_load_explicit(&hero_signal_returns[0], memory_order_relaxed);
    uintptr_t info = atomic_load_explicit(&hero_signal_returns[1], memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[1], info != 0 ? info : plain, memory_order_relaxed);
    atomic_store_explicit(&hero_signal_returns[0], plain != 0 ? plain : info, memory_order_release);
    atomic_store_explicit(&hero_signal_state, 2, memory_order_release);
}

/* THE THREAD'S OWN STACK, read once as the thread is claimed (`parts/thread.c`:
 * `main`'s from `hero_args_set`, a worker's from `hero_spawn_enter` while its
 * signals are still held off), never inside a signal: glibc's
 * `pthread_getattr_np` allocates. One record, so a thunk reaches both bounds
 * through one thread-local access, a call of Darwin's accessor: four of them a
 * thunk cost a `qsort` comparator 3.7 times D's instructions (panel 247's
 * compiler-engineer). Asked of the system rather than of `parts/stack.c`'s
 * guard, which `--sanitize` empties and the walk was then blind (the same). A
 * thread C made is never claimed and keeps 0 and 0: every frame lies outside
 * it, the kernel is asked, and the thread's guard refuses the function after. */
typedef struct {
    uintptr_t lo;
    uintptr_t hi;
} HeroSignalStack;

static _Thread_local HeroSignalStack hero_signal_stack = {0, 0};

static void hero_signal_measure(void) {
#if defined(__APPLE__)
    pthread_t self = pthread_self();
    uintptr_t top = (uintptr_t)pthread_get_stackaddr_np(self);
    hero_signal_stack.lo = top - pthread_get_stacksize_np(self);
    hero_signal_stack.hi = top;
#else
    pthread_attr_t attr;
    void *addr = NULL;
    size_t size = 0;
    if (pthread_getattr_np(pthread_self(), &attr) == 0) {
        pthread_attr_getstack(&attr, &addr, &size);
        pthread_attr_destroy(&attr);
    }
    hero_signal_stack.lo = (uintptr_t)addr;
    hero_signal_stack.hi = (uintptr_t)addr + size;
#endif
}

/* The refusal: `delivered` where the system called the function itself, else
 * a handler of C's called it. */
static _Noreturn void hero_signal_refuse(const char *what, int delivered) {
    const char *const said[] = {
        "panic: ",
        what,
        delivered ? " ran as a signal handler: the system called it inside a signal, where neither this "
                    "runtime's memory nor the C library's is safe to use. "
                  : " was called from inside a signal handler: C called it while handling a signal, where "
                    "neither this runtime's memory nor the C library's is safe to use. ",
        HERO_SIGNAL_REMEDY,
    };
    hero_signal_stop(said, sizeof said / sizeof said[0]);
}

/* The thunk's first line: returns where no signal's delivery is found, and
 * otherwise does not return. `ret` and `frame` are the thunk's own
 * `__builtin_return_address(0)` and `__builtin_frame_address(0)`. Each read
 * of the walk is a word between the thunk's frame and the top of its thread's
 * stack, above the stack pointer, so it reads memory the thread has used. */
void hero_callback_entered(const char *what, void *ret, void *frame) {
    uintptr_t plain = atomic_load_explicit(&hero_signal_returns[0], memory_order_acquire);
    if (plain == 0) return;
    uintptr_t info = atomic_load_explicit(&hero_signal_returns[1], memory_order_relaxed);
    uintptr_t back = (uintptr_t)ret;
    if (back == plain || back == info) hero_signal_refuse(what, 1);
    const HeroSignalStack *own = &hero_signal_stack;
    uintptr_t lo = own->lo;
    uintptr_t hi = own->hi;
    uintptr_t fp = (uintptr_t)frame;
    if (fp < lo || fp >= hi) {
        stack_t now;
        if (sigaltstack(NULL, &now) == 0 && (now.ss_flags & SS_ONSTACK) != 0) hero_signal_refuse(what, 0);
        return;
    }
    /* A record is two words, so the highest one the walk may read starts at
     * `top`; each step climbs, so the walk ends inside the stack. */
    uintptr_t top = hi - 2 * sizeof(uintptr_t);
    for (int up = 0; up < HERO_SIGNAL_WALK; up += 1) {
        uintptr_t next = ((const uintptr_t *)fp)[0];
        if (next <= fp || next > top || (next & (sizeof(uintptr_t) - 1)) != 0) return;
        uintptr_t above = ((const uintptr_t *)next)[1];
        if (above == plain || above == info) hero_signal_refuse(what, 0);
        fp = next;
    }
}

#else

/* Windows: no probe and no walk (panel 247's R10). A console's Ctrl-C runs its
 * handler on a thread Windows makes, which `parts/thread.c`'s guard refuses,
 * and a fault's on the faulting thread, which D's guard stands under; whether
 * UCRT's `raise` leaves a delivery's address to learn is the box's question,
 * unrun. */
void hero_signal_learn(void) {}

void hero_callback_entered(const char *what, void *ret, void *frame) {
    (void)what;
    (void)ret;
    (void)frame;
}

static void hero_signal_measure(void) {}

#endif

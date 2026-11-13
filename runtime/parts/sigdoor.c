/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigdoor.c — the runtime's door for a signal other than SIGINT and
 * SIGTERM (panel 247's G; `hero_os.h` declares it), whose door is the platform
 * layer's stop (`hero_net.h`, panel 246's R4).
 *
 * WHY THE RUNTIME HOLDS IT. No Heroes code runs inside a signal handler (panel
 * 246's R4, `parts/sigguard.c`), so a program that wants to know a signal came
 * needs a handler that is C's. The handler is this file's and nothing else: it
 * counts the delivery in one process-wide counter per signal and writes one
 * byte down a pipe, both async-signal-safe; the program asks between its steps
 * (`hero_signal_take`) or waits on the descriptor with `poll`
 * (`hero_signal_fd`). Built and run by panel 247's ffi-pragmatist: a program
 * counting SIGUSR1 under a storm, and another whose second module takes the
 * count, both exit 0 on Darwin and Linux arm64.
 *
 * ONE COUNTER PER PROCESS, in this one translation unit, so two modules that
 * both bind `hero_os.h` read the same count: a `static` flag in a header two
 * modules include is two flags, and the second module's was never read (panel
 * 247's spec-warden, `swp6`, SIGINT ignored 2 runs of 2 at exit 0).
 *
 * WHAT A COUNT IS: deliveries, never sends. The kernel merges a standard
 * signal sent while one is pending, so a storm is counted short of what was
 * sent (the sitting measured a quarter to three quarters merged); a signal the
 * program raises on its own thread is delivered before `raise` returns, and
 * each is counted. While `heroes run`'s runner waits on a child it holds
 * SIGHUP and SIGQUIT itself, to pass them on (`parts/run.c`), and puts the
 * door back after; SIGURG is borrowed for an instant by `parts/sigguard.c`'s
 * probe, the first time the program hands C a function, and put back. */

#if !defined(_WIN32)
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

/* Every signal number POSIX's two platforms use, Linux's real-time ones
 * included (64 at most there, 31 on Darwin). */
#define HERO_SIGDOOR_MAX 65

static _Atomic int64_t hero_sigdoor_count[HERO_SIGDOOR_MAX];

/* The pipe, made once by the first watch; -1 until then. */
static _Atomic int hero_sigdoor_fds[2] = {-1, -1};

/* The handler: the count, then one byte, `errno` kept for the code it
 * interrupted. A full pipe drops the byte, and the count keeps the delivery. */
static void hero_sigdoor_on(int sig) {
    int saved = errno;
    if (sig > 0 && sig < HERO_SIGDOOR_MAX) atomic_fetch_add_explicit(&hero_sigdoor_count[sig], 1, memory_order_relaxed);
    int fd = atomic_load_explicit(&hero_sigdoor_fds[1], memory_order_acquire);
    if (fd >= 0) {
        unsigned char b = (unsigned char)sig;
        (void)!write(fd, &b, 1);
    }
    errno = saved;
}

/* The pipe, both ends non-blocking and closed across `exec`; one thread makes
 * it, and a second watching at the same moment gives its own back. */
static int hero_sigdoor_pipe(void) {
    if (atomic_load_explicit(&hero_sigdoor_fds[0], memory_order_acquire) >= 0) return 0;
    int fds[2];
    if (pipe(fds) != 0) return -1;
    for (int i = 0; i < 2; i++) {
        fcntl(fds[i], F_SETFD, FD_CLOEXEC);
        fcntl(fds[i], F_SETFL, fcntl(fds[i], F_GETFL) | O_NONBLOCK);
    }
    int none = -1;
    if (!atomic_compare_exchange_strong(&hero_sigdoor_fds[0], &none, fds[0])) {
        close(fds[0]);
        close(fds[1]);
        while (atomic_load_explicit(&hero_sigdoor_fds[1], memory_order_acquire) < 0) sched_yield();
        return 0;
    }
    atomic_store_explicit(&hero_sigdoor_fds[1], fds[1], memory_order_release);
    return 0;
}

/* 0, or -1 for a signal this door does not take: a fault's (the runtime's
 * stack guard and lease handler own them, and returning from one runs the
 * faulting instruction again), SIGKILL and SIGSTOP (no handler can take them),
 * SIGINT and SIGTERM (the platform layer's stop, `hero_net.h`), and a number
 * no signal has; or where the system gave no pipe or refused the handler. */
int64_t hero_signal_watch(int32_t sig) {
    if (sig <= 0 || sig >= HERO_SIGDOOR_MAX || sig == SIGSEGV || sig == SIGBUS || sig == SIGFPE || sig == SIGILL ||
        sig == SIGTRAP || sig == SIGABRT || sig == SIGSYS || sig == SIGKILL || sig == SIGSTOP || sig == SIGINT ||
        sig == SIGTERM)
        return -1;
    if (hero_sigdoor_pipe() != 0) return -1;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = hero_sigdoor_on;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL) == 0 ? 0 : -1;
}

/* The deliveries counted since the last take, and the count set back to 0. */
int64_t hero_signal_take(int32_t sig) {
    if (sig <= 0 || sig >= HERO_SIGDOOR_MAX) return 0;
    return atomic_exchange_explicit(&hero_sigdoor_count[sig], 0, memory_order_relaxed);
}

/* Readable once a watched signal has come, for a program that waits with
 * `poll`; -1 before the first watch. */
int32_t hero_signal_fd(void) { return atomic_load_explicit(&hero_sigdoor_fds[0], memory_order_acquire); }

#else

/* Windows has nothing for this door to watch: UCRT raises SIGINT and SIGBREAK
 * for the console's events, the stop's (`hero_net.h`), never generates SIGILL
 * or SIGTERM, and its other signals are faults (Microsoft's `signal` page,
 * read by panel 247's ffi-pragmatist). So every signal is one it does not
 * take, nothing is counted, and there is no descriptor. Compiled for both of
 * Windows' targets by zig, never run on the box. */
int64_t hero_signal_watch(int32_t sig) {
    (void)sig;
    return -1;
}

int64_t hero_signal_take(int32_t sig) {
    (void)sig;
    return 0;
}

int32_t hero_signal_fd(void) { return -1; }

#endif

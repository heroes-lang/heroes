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
 * (`hero_signal_fd`, `hero_signal_door`). Built and run by panel 247's ffi-pragmatist: a program
 * counting SIGUSR1 under a storm, and another whose second module takes the
 * count, both exit 0 on Darwin and Linux arm64.
 *
 * ONE COUNTER PER PROCESS, in this one translation unit, so two modules that
 * both bind `hero_os.h` read the same count: a `static` flag in a header two
 * modules include is two flags, and the second module's was never read (panel
 * 247's spec-warden, `swp6`, SIGINT ignored 2 runs of 2 at exit 0).
 *
 * SIGINT AND SIGTERM ARE COUNTED HERE ONLY AS THE PLATFORM LAYER HANDS THEM
 * OVER (feature 2669). `hero_signal_watch` refuses them, as panel 247's R7
 * rules: the two are the stop's, and the stop is the layer's. A program that
 * wants each Ctrl-C counted and none to end it, a shell or a REPL, asks the
 * layer (`hero_net_count_signal`), which holds the stop and the count apart
 * under its own lock and then calls `hero_signal_take_over` below
 * (`platform/hero_layer_signal.h`): the counter, the pipe and
 * `hero_signal_take` are this file's, one for every signal. On Windows the
 * console's handler counts Ctrl-C and Ctrl-Break as SIGINT through
 * `hero_signal_count_one`, on the thread the system makes for it.
 *
 * WHAT A COUNT IS: deliveries, never sends. The kernel merges a standard
 * signal sent while one is pending, so a storm is counted short of what was
 * sent (the sitting measured a quarter to three quarters merged); a signal the
 * program raises on its own thread is delivered before `raise` returns, and
 * each is counted. While `heroes run`'s runner waits on a child it holds
 * SIGHUP and SIGQUIT itself, to pass them on (`parts/run.c`), and puts the
 * door back after; SIGURG is borrowed for an instant by `parts/sigguard.c`'s
 * probe, the first time the program hands C a function, and put back.
 *
 * A SIGNAL THAT REACHES A CHILD FORKED BEFORE ITS `exec` IS THE CHILD'S, as
 * the stop's handler rules for its two (`hero_net.c`): such a child shares
 * this handler and the pipe until its `exec`, so the handler counts only in
 * the process that installed it and, anywhere else, puts the signal's default
 * back and sends it again, so a Ctrl-C meant for a child that has not yet
 * become its program ends it, or stops it, as it would have, and no byte of
 * the child's reaches this process's pipe.
 *
 * THE DESCRIPTOR IS READABLE WHILE A DELIVERY IS COUNTED AND NOT TAKEN
 * (feature 2690), so a wait on it ends once for each signal a program takes,
 * and every waiter sees it: nobody drains it but the count's reading. Each
 * `hero_signal_take` drains the pipe and then writes one byte back where a
 * count of any signal is still above 0. The handler counts before it writes,
 * so a byte the drain takes belongs to a count the check after it sees, and a
 * byte written after the drain stays; what can be left is a byte with no
 * count behind it, which `hero_signal_any` drains when it finds none. A wait
 * that would end on one signal and not another reads the counts it wants, as
 * `hero_signal_any` reads them all. On Windows the descriptor is a manual-reset
 * event the console's handler sets, settled the same way.
 *
 * WHAT A NAME IS: `hero_signal_number` gives the number this platform gives a
 * signal `hero_os.h` names the same on every platform (SIGTSTP is 18 on
 * Darwin and 20 on Linux), read from `<signal.h>` where it is compiled. */

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

/* The process that installed the handler, published before the handler is:
 * a forked child sees another pid of its own. */
static _Atomic int hero_sigdoor_owner = -1;

/* The handler: in the process that installed it, the count, then one byte,
 * `errno` kept for the code it interrupted; a full pipe drops the byte, and
 * the count keeps the delivery. In a forked child before its `exec`, the
 * signal's default back and the signal sent again, which is the child's. */
static void hero_sigdoor_on(int sig) {
    int saved = errno;
    if ((int)getpid() != atomic_load_explicit(&hero_sigdoor_owner, memory_order_acquire)) {
        struct sigaction plain = { 0 };
        plain.sa_handler = SIG_DFL;
        sigemptyset(&plain.sa_mask);
        (void)sigaction(sig, &plain, NULL);
        (void)kill(getpid(), sig);
        errno = saved;
        return;
    }
    if (sig > 0 && sig < HERO_SIGDOOR_MAX) atomic_fetch_add_explicit(&hero_sigdoor_count[sig], 1, memory_order_release);
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

/* The handler installed for `sig`, the pipe made first and the owner
 * published before either: 0, or -1 where the system gave no pipe or refused
 * the handler. */
static int64_t hero_sigdoor_install(int sig) {
    if (hero_sigdoor_pipe() != 0) return -1;
    atomic_store_explicit(&hero_sigdoor_owner, (int)getpid(), memory_order_release);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = hero_sigdoor_on;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL) == 0 ? 0 : -1;
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
    return hero_sigdoor_install(sig);
}

/* The two the stop owns, handed over by the platform layer under its lock
 * once it has refused the stop for them (`hero_net_count_signal`). */
int64_t hero_signal_take_over(int32_t sig) {
    if (sig != SIGINT && sig != SIGTERM) return -1;
    return hero_sigdoor_install(sig);
}

/* One delivery counted from outside a handler; POSIX counts in the handler,
 * so the platform layer never calls this here. */
void hero_signal_count_one(int32_t sig) {
    if (sig > 0 && sig < HERO_SIGDOOR_MAX) atomic_fetch_add_explicit(&hero_sigdoor_count[sig], 1, memory_order_release);
}

/* Whether a delivery of any signal is counted and not taken. */
static bool hero_sigdoor_untaken(void) {
    for (int sig = 1; sig < HERO_SIGDOOR_MAX; sig++) {
        if (atomic_load_explicit(&hero_sigdoor_count[sig], memory_order_acquire) > 0) return true;
    }
    return false;
}

/* The pipe drained, then one byte written back where a count is still above
 * 0: readable from here exactly while a delivery is untaken. Nothing before
 * the pipe is made. */
static void hero_sigdoor_settle(void) {
    int in = atomic_load_explicit(&hero_sigdoor_fds[0], memory_order_acquire);
    int out = atomic_load_explicit(&hero_sigdoor_fds[1], memory_order_acquire);
    if (in < 0 || out < 0) return;
    unsigned char sink[64];
    for (;;) {
        ssize_t got = read(in, sink, sizeof sink);
        if (got > 0 || (got < 0 && errno == EINTR)) continue;
        break;
    }
    if (hero_sigdoor_untaken()) {
        unsigned char b = 0;
        (void)!write(out, &b, 1);
    }
}

/* The deliveries counted since the last take, the count set back to 0 and the
 * descriptor settled. */
int64_t hero_signal_take(int32_t sig) {
    if (sig <= 0 || sig >= HERO_SIGDOOR_MAX) return 0;
    int64_t taken = atomic_exchange_explicit(&hero_sigdoor_count[sig], 0, memory_order_acq_rel);
    hero_sigdoor_settle();
    return taken;
}

/* 1 where a delivery of any signal is counted and not taken; 0 where none
 * is, a byte left with no count behind it drained first. */
int64_t hero_signal_any(void) {
    if (hero_sigdoor_untaken()) return 1;
    hero_sigdoor_settle();
    return hero_sigdoor_untaken() ? 1 : 0;
}

/* Readable while a counted delivery is untaken, for a program that waits with
 * `poll`; -1 before the first watch. */
int32_t hero_signal_fd(void) { return atomic_load_explicit(&hero_sigdoor_fds[0], memory_order_acquire); }

/* The same descriptor, the pipe made first where no watch made it yet, so a
 * wait may hold it before the program counts anything: -1 where the system
 * gave no pipe. */
int32_t hero_signal_door(void) {
    if (hero_sigdoor_pipe() != 0) return -1;
    return atomic_load_explicit(&hero_sigdoor_fds[0], memory_order_acquire);
}

/* POSIX waits on the descriptor: no event. */
void *hero_signal_door_event(void) { return NULL; }

#else

/* Windows has nothing for `hero_signal_watch` to watch: UCRT raises SIGINT and
 * SIGBREAK for the console's events, which are the platform layer's (the
 * stop's, or counted as SIGINT where the program asked the layer to count
 * Ctrl-C), never generates SIGILL or SIGTERM, and its other signals are faults
 * (Microsoft's `signal` page, read by panel 247's ffi-pragmatist). So every
 * signal is one the watch does not take, and there is no descriptor; the one
 * count is SIGINT's, which the console's handler adds to on the thread the
 * system makes for it (`hero_net.c`). Compiled for both of Windows' targets by
 * zig, never run on the box. A wait that ends on a counted Ctrl-C waits on
 * a manual-reset event, set by each count and settled by each take as the
 * POSIX pipe is (`hero_signal_door_event`). */
#include <signal.h>
#include <windows.h>

/* UCRT's numbers stop at SIGABRT_COMPAT, 6, and SIGBREAK, 21. */
#define HERO_SIGDOOR_MAX 23

static _Atomic int64_t hero_sigdoor_count[HERO_SIGDOOR_MAX];

/* The event, made by the first wait that asks for it; NULL until then. */
static _Atomic(void *) hero_sigdoor_event = NULL;

static bool hero_sigdoor_untaken(void) {
    for (int sig = 1; sig < HERO_SIGDOOR_MAX; sig++) {
        if (atomic_load_explicit(&hero_sigdoor_count[sig], memory_order_acquire) > 0) return true;
    }
    return false;
}

/* The event reset, then set again where a count is still above 0. */
static void hero_sigdoor_settle(void) {
    void *event = atomic_load_explicit(&hero_sigdoor_event, memory_order_acquire);
    if (event == NULL) return;
    (void)ResetEvent(event);
    if (hero_sigdoor_untaken()) (void)SetEvent(event);
}

int64_t hero_signal_watch(int32_t sig) {
    (void)sig;
    return -1;
}

/* No handler is installed for a console's events: the layer's handler counts
 * them itself. */
int64_t hero_signal_take_over(int32_t sig) {
    (void)sig;
    return -1;
}

/* Counted, then the event set, on the thread the system makes for the
 * console's event. */
void hero_signal_count_one(int32_t sig) {
    if (sig <= 0 || sig >= HERO_SIGDOOR_MAX) return;
    atomic_fetch_add_explicit(&hero_sigdoor_count[sig], 1, memory_order_release);
    void *event = atomic_load_explicit(&hero_sigdoor_event, memory_order_acquire);
    if (event != NULL) (void)SetEvent(event);
}

int64_t hero_signal_take(int32_t sig) {
    if (sig <= 0 || sig >= HERO_SIGDOOR_MAX) return 0;
    int64_t taken = atomic_exchange_explicit(&hero_sigdoor_count[sig], 0, memory_order_acq_rel);
    hero_sigdoor_settle();
    return taken;
}

int64_t hero_signal_any(void) {
    if (hero_sigdoor_untaken()) return 1;
    hero_sigdoor_settle();
    return hero_sigdoor_untaken() ? 1 : 0;
}

int32_t hero_signal_fd(void) { return -1; }

int32_t hero_signal_door(void) { return -1; }

/* The event, made once: a second thread making it at the same moment closes
 * its own. Set at once where a count is already above 0, so a count that came
 * before the event is not missed. NULL where the system gave none. */
void *hero_signal_door_event(void) {
    void *event = atomic_load_explicit(&hero_sigdoor_event, memory_order_acquire);
    if (event != NULL) return event;
    void *made = CreateEventW(NULL, TRUE, FALSE, NULL);
    if (made == NULL) return NULL;
    void *none = NULL;
    if (!atomic_compare_exchange_strong(&hero_sigdoor_event, &none, made)) {
        (void)CloseHandle(made);
        return none;
    }
    if (hero_sigdoor_untaken()) (void)SetEvent(made);
    return made;
}

#endif

/* The platform's number for each name `hero_os.h` gives, from the platform's
 * own `<signal.h>`; -1 for a signal that never comes here. One table for both
 * arms, so a name is answered the same way wherever the header has it. */
int32_t hero_signal_number(int32_t name) {
    switch (name) {
#if !defined(_WIN32)
        case HERO_SIGNAL_HANGUP: return SIGHUP;
        case HERO_SIGNAL_INTERRUPT: return SIGINT;
        case HERO_SIGNAL_QUIT: return SIGQUIT;
        case HERO_SIGNAL_KILL: return SIGKILL;
        case HERO_SIGNAL_USER1: return SIGUSR1;
        case HERO_SIGNAL_USER2: return SIGUSR2;
        case HERO_SIGNAL_TERMINATE: return SIGTERM;
        case HERO_SIGNAL_CHILD: return SIGCHLD;
        case HERO_SIGNAL_CONTINUE: return SIGCONT;
        case HERO_SIGNAL_STOP: return SIGSTOP;
        case HERO_SIGNAL_TERMINAL_STOP: return SIGTSTP;
        case HERO_SIGNAL_TERMINAL_INPUT: return SIGTTIN;
        case HERO_SIGNAL_TERMINAL_OUTPUT: return SIGTTOU;
        case HERO_SIGNAL_WINDOW: return SIGWINCH;
#else
        /* UCRT's SIGINT is what the console's Ctrl-C and Ctrl-Break are
         * counted as; its SIGTERM is never generated, and no other of these
         * exists for a console program. */
        case HERO_SIGNAL_INTERRUPT: return SIGINT;
#endif
        default: return -1;
    }
}

/* The C the cases of the platform layer's stop share (panel 246's R4; lane
 * b34-layer, 2026-11-13): a stop sent to the program by a thread of its own,
 * as `kill`, a terminal or a service manager sends one, and the program's
 * start made what each case needs, since a harness started in the background
 * hands its programs SIGINT ignored, which the stop keeps on purpose.
 *
 * AN INTERRUPT is SIGINT on POSIX and Ctrl-C on Windows; A REQUEST TO END is
 * SIGTERM, and on Windows Ctrl-Break, the console's other event. On POSIX the
 * signal goes to the process, `kill(getpid(), ...)`, from a thread that holds
 * both signals blocked, so the system runs the handler on another thread: the
 * sitting's critic's shape, `main` waiting in a join while the `accept` waits
 * on a worker. On Windows a console event reaches every process of the
 * console, the harness's own among them, so `stop_begin` first gives the
 * program a console of its own that no other process shares, and the event is
 * generated there; the program's output is the file the harness gave it, which
 * the C library holds from its start whatever console stands.
 *
 * A stop that does not end the waiting `accept` within 5 s is the case's
 * failure, never a hang: the sending thread then connects to the port the
 * worker noted, so the `accept` returns and the program ends either way. */
#include "hero_net.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#endif

#define STOP_INTERRUPT 1
#define STOP_REQUEST 2

static _Atomic int64_t stop_port;
static _Atomic int stop_finished;
static int64_t stop_what;
static int64_t stop_ms;

static void stop_pause(int64_t ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec t = { (time_t)(ms / 1000), (long)(ms % 1000) * 1000000L };
    nanosleep(&t, NULL);
#endif
}

/* The program's start, made the case's: the request to end at its default,
 * the interrupt at its default or ignored (`interrupt_ignored` 1), as a shell
 * leaves it for a job it starts in the background. 1, or the negative step
 * that failed. */
static inline int64_t stop_begin(int64_t interrupt_ignored) {
#if defined(_WIN32)
    (void)FreeConsole();
    if (!AllocConsole()) return -1;
    if (!SetConsoleCtrlHandler(NULL, interrupt_ignored ? TRUE : FALSE)) return -2;
#else
    if (signal(SIGTERM, SIG_DFL) == SIG_ERR) return -1;
    if (signal(SIGINT, interrupt_ignored ? SIG_IGN : SIG_DFL) == SIG_ERR) return -2;
#endif
    return 1;
}

/* The stop sent now, to the whole program. 1, or -1 where it could not be. */
static inline int64_t stop_send_now(int64_t what) {
    fflush(stdout);
#if defined(_WIN32)
    if (!GenerateConsoleCtrlEvent(what == STOP_INTERRUPT ? CTRL_C_EVENT : CTRL_BREAK_EVENT, 0)) return -1;
    /* Windows runs a console's handlers on a thread it makes, after this
     * returns: the event is given time to land. */
    stop_pause(300);
#else
    if (kill(getpid(), what == STOP_INTERRUPT ? SIGINT : SIGTERM) != 0) return -1;
#endif
    return 1;
}

/* The worker's listening port, and that its `accept` returned. */
static inline void stop_note_port(int64_t port) { atomic_store(&stop_port, port); }
static inline void stop_note_done(void) { atomic_store(&stop_finished, 1); }

static void stop_sender(void) {
    stop_pause(stop_ms);
    (void)stop_send_now(stop_what);
    for (int waited = 0; waited < 5000 && !atomic_load(&stop_finished); waited += 10) {
        stop_pause(10);
    }
    int64_t port = atomic_load(&stop_port);

    if (!atomic_load(&stop_finished) && port > 0) {
        hero_net_socket *freeing = hero_net_connect("127.0.0.1", port);

        stop_pause(500);
        if (freeing != NULL) hero_net_close(freeing);
    }
}

#if defined(_WIN32)
static DWORD WINAPI stop_sender_body(LPVOID unused) {
    (void)unused;
    stop_sender();
    return 0;
}
#else
static void *stop_sender_body(void *unused) {
    (void)unused;
    sigset_t both;

    sigemptyset(&both);
    sigaddset(&both, SIGINT);
    sigaddset(&both, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &both, NULL);
    stop_sender();
    return NULL;
}
#endif

/* The stop sent `ms` from now by a thread of the program's own, which then
 * waits for the worker's `accept` and frees it where the stop did not. 1, or
 * -1 where the thread could not start. */
static inline int64_t stop_send_after(int64_t what, int64_t ms) {
    stop_what = what;
    stop_ms = ms;
#if defined(_WIN32)
    HANDLE t = CreateThread(NULL, 0, stop_sender_body, NULL, 0, NULL);

    if (t == NULL) return -1;
    CloseHandle(t);
#else
    pthread_t t;

    if (pthread_create(&t, NULL, stop_sender_body, NULL) != 0) return -1;
    pthread_detach(t);
#endif
    return 1;
}

/* The program's output so far, written out before a signal ends it. */
static inline void stop_flush(void) { fflush(stdout); }

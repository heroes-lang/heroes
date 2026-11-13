/* Defect 2494: the program's own C for its case. A thread C starts with
 * `pthread_create`, which the runtime never claims, waits with SIGUSR1 let
 * through while the thread that started it holds it off; a handler written in
 * C calls the Heroes function it was handed, and one SIGUSR1 is sent to the
 * thread C made. */
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static int64_t (*made_answer)(int64_t) = NULL;
static volatile sig_atomic_t made_got = 0;
static pthread_t made_thread;
static atomic_int made_ready = 0;

static void made_handler(int sig) { made_got = (sig_atomic_t)made_answer((int64_t)sig); }

static void *made_body(void *unused) {
    (void)unused;
    sigset_t one;
    sigemptyset(&one);
    sigaddset(&one, SIGUSR1);
    pthread_sigmask(SIG_UNBLOCK, &one, NULL);
    atomic_store(&made_ready, 1);
    for (;;) pause();
    return NULL;
}

/* The handler installed, SIGUSR1 held off the calling thread, and the thread
 * C makes started and letting it through: 0, or -1. */
static inline int32_t made_start(int64_t (*answer)(int64_t)) {
    struct sigaction sa;
    made_answer = answer;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = made_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    sigset_t one;
    sigemptyset(&one);
    sigaddset(&one, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &one, NULL);
    if (pthread_create(&made_thread, NULL, made_body, NULL) != 0) return -1;
    while (!atomic_load(&made_ready)) sched_yield();
    return 0;
}

static inline int32_t made_send(void) { return pthread_kill(made_thread, SIGUSR1); }

/* `main` waits for the stop, at most ten seconds, and says if none came. */
static inline int64_t made_wait(void) {
    for (int i = 0; i < 100 && made_got == 0; i++) usleep(100000);
    return (int64_t)made_got;
}

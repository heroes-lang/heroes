/* Defect 2494: the program's own C for its case, the lend tracking's twin of
 * `fixedbugs-2494-a-thread-of-cs-own-sends-the-signal.h`. A thread C starts
 * sends SIGUSR1 to the thread that started it as fast as `pthread_kill`
 * returns, for at most 20 seconds or until told to stop, while that thread
 * lends a buffer to C; the thread sending holds SIGUSR1 itself, so it never
 * runs the handler.
 *
 * The handler is C (panel 247's R8): it calls the Heroes function it was
 * handed through DEEP_FRAMES frames of its own, more than the thunk's walk
 * reads (eight, `runtime/parts/sigguard.c`), each kept by `noinline` and
 * `disable_tail_calls`, so the call reaches the function past the thunk's
 * question, and the lend tracking's guard is what stands there (panel 247's
 * R2). It sends nothing until `lender_start` has made its last C library
 * call, for the twin's reason. */
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define DEEP_FRAMES 24

static pthread_t lender_target;
static void (*lender_called)(int32_t) = NULL;
static volatile int lender_depth_seen = 0;

__attribute__((noinline, disable_tail_calls)) static int lender_down(int left, int sig) {
    if (left == 0) {
        lender_called((int32_t)sig);
        return 0;
    }
    int below = lender_down(left - 1, sig);
    lender_depth_seen = left;
    return below + 1;
}

static void lender_handler(int sig) { (void)lender_down(DEEP_FRAMES, sig); }

/* The handler, installed for SIGUSR1 with `sigaction`: 0, or -1 where the
 * system refused it. */
static inline int32_t lender_install(void (*called)(int32_t)) {
    struct sigaction sa;
    lender_called = called;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = lender_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(SIGUSR1, &sa, NULL) == 0 ? 0 : -1;
}

/* What a lend reaches: the cell C was lent, one more. */
static inline int32_t lender_touch(int32_t *cell) {
    *cell += 1;
    return *cell;
}

/* What `main`'s lend reaches: `n` bytes C fills. */
static inline int32_t lender_fill(uint8_t *buf, uint64_t n) {
    for (uint64_t i = 0; i < n; i++) buf[i] = (uint8_t)i;
    return (int32_t)n;
}

static atomic_int lender_go = 0;
static atomic_int lender_done = 0;

static void *lender_send(void *unused) {
    (void)unused;
    while (!atomic_load(&lender_go)) sched_yield();
    struct timespec began;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &began);
    for (long sent = 1; !atomic_load(&lender_done); sent += 1) {
        if (pthread_kill(lender_target, SIGUSR1) != 0) break;
        if ((sent & 1023) == 0) {
            clock_gettime(CLOCK_MONOTONIC, &now);
            if (now.tv_sec - began.tv_sec >= 20) break;
        }
    }
    return 0;
}

/* 1 once the sender is aimed at the calling thread and let go; 0 where it
 * could not be started. */
static inline int64_t lender_start(void) {
    lender_target = pthread_self();
    sigset_t usr1;
    sigset_t was;
    sigemptyset(&usr1);
    sigaddset(&usr1, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &usr1, &was);
    pthread_t sender;
    int made = pthread_create(&sender, NULL, lender_send, NULL);
    pthread_sigmask(SIG_SETMASK, &was, NULL);
    if (made != 0) return 0;
    pthread_detach(sender);
    atomic_store(&lender_go, 1);
    return 1;
}

static inline void lender_stop(void) {
    atomic_store(&lender_done, 1);
}

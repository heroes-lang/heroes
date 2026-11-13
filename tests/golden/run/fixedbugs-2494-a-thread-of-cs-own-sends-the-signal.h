/* Defect 2494: the program's own C for its case. A thread C starts sends
 * SIGUSR1 to the thread that started it as fast as `pthread_kill` returns,
 * for at most 20 seconds or until told to stop, while that thread allocates;
 * the thread sending holds SIGUSR1 itself, so it never runs the handler.
 *
 * The handler is C (panel 247's R8): it calls the Heroes function it was
 * handed through DEEP_FRAMES frames of its own, more than the thunk's walk
 * reads (eight, `runtime/parts/sigguard.c`), each kept by `noinline` and
 * `disable_tail_calls`, so the call reaches the function past the thunk's
 * question, as a handler whose frames keep no record or lie deeper does, and
 * the allocator's guard is what stands there (panel 247's R2).
 *
 * It sends nothing until `storm_start` has made its last C library call:
 * creating a thread allocates through the C library directly, outside the
 * runtime's brackets, and a signal landing there is the part of the defect
 * the repair leaves to panel 247 (5 runs of 200 on this Mac were killed with
 * nothing said, the case's first shape). */
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define DEEP_FRAMES 24

static pthread_t storm_target;
static void (*storm_called)(int32_t) = NULL;
static volatile int storm_depth_seen = 0;

__attribute__((noinline, disable_tail_calls)) static int storm_down(int left, int sig) {
    if (left == 0) {
        storm_called((int32_t)sig);
        return 0;
    }
    int below = storm_down(left - 1, sig);
    storm_depth_seen = left;
    return below + 1;
}

static void storm_handler(int sig) { (void)storm_down(DEEP_FRAMES, sig); }

/* The handler, installed for SIGUSR1 with `sigaction`: 0, or -1 where the
 * system refused it. */
static inline int32_t storm_install(void (*called)(int32_t)) {
    struct sigaction sa;
    storm_called = called;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = storm_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(SIGUSR1, &sa, NULL) == 0 ? 0 : -1;
}

static atomic_int storm_go = 0;
static atomic_int storm_done = 0;

static void *storm_send(void *unused) {
    (void)unused;
    while (!atomic_load(&storm_go)) sched_yield();
    struct timespec began;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &began);
    for (long sent = 1; !atomic_load(&storm_done); sent += 1) {
        if (pthread_kill(storm_target, SIGUSR1) != 0) break;
        if ((sent & 1023) == 0) {
            clock_gettime(CLOCK_MONOTONIC, &now);
            if (now.tv_sec - began.tv_sec >= 20) break;
        }
    }
    return 0;
}

/* 1 once the sender is aimed at the calling thread and let go; 0 where it
 * could not be started. */
static inline int64_t storm_start(void) {
    storm_target = pthread_self();
    sigset_t usr1;
    sigset_t was;
    sigemptyset(&usr1);
    sigaddset(&usr1, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &usr1, &was);
    pthread_t sender;
    int made = pthread_create(&sender, NULL, storm_send, NULL);
    pthread_sigmask(SIG_SETMASK, &was, NULL);
    if (made != 0) return 0;
    pthread_detach(sender);
    atomic_store(&storm_go, 1);
    return 1;
}

static inline void storm_stop(void) {
    atomic_store(&storm_done, 1);
}

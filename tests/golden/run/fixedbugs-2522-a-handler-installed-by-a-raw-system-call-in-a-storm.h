/* Defect 2522 (panel 251's R1): the program's own C for its cases, defect
 * 2494's storm (`fixedbugs-2494-a-thread-of-cs-own-sends-the-signal.h`) with
 * one difference: the handler is installed by a raw system call
 * (`fixedbugs-2522-a-raw-install.h`), which no route of the runtime sees. It
 * was installed through the `sigaction` `dlsym` found in the C library's
 * image until the author's ruling C of 2026-11-20 made `dlsym` answer the
 * runtime's own (lane-signals-2). So the thunk's route has no entry for this
 * handler, and the allocator's and the lend tracking's guards are what stand
 * there, as they stood before the routes (panel 247's R2).
 *
 * A thread C starts sends SIGUSR1 to the thread that started it as fast as
 * `pthread_kill` returns, for at most 20 seconds or until told to stop, while
 * that thread allocates; the thread sending holds SIGUSR1 itself, so it never
 * runs the handler. The handler calls the Heroes function it was handed
 * through DEEP_FRAMES frames of its own, more than the thunk's walk reads.
 * It sends nothing until `storm_start` has made its last C library call. */
#include <unistd.h>
#include "fixedbugs-2522-a-raw-install.h"
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

/* The handler, installed for SIGUSR1 by a raw system call: 0, or -1 where
 * the system refused. */
static inline int32_t storm_install(void (*called)(int32_t)) {
    storm_called = called;
    return raw_install(SIGUSR1, storm_handler) == 0 ? 0 : -1;
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

/* Defect 2496: the program's own C for its case. It holds SIGUSR1 off the
 * calling thread alone, sends SIGUSR1 to the whole process, and its handler,
 * written in C since no Heroes function may be one (panel 247's R1 and R8),
 * counts its runs in a lock-free atomic, which a signal handler may touch,
 * and keeps the thread it ran on, so a worker can ask whether it was itself
 * (panel 247's compiler-engineer: the count alone holds whichever thread ran
 * the handler). */
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdint.h>
#include <unistd.h>

static _Atomic int64_t fresh_heard = 0;

/* SIGUSR1 held off the calling thread, the others untouched. */
static inline int64_t fresh_hold(void) {
    sigset_t one;
    sigemptyset(&one);
    sigaddset(&one, SIGUSR1);
    return pthread_sigmask(SIG_BLOCK, &one, NULL) == 0 ? 1 : 0;
}

/* And let through again: one still pending is handled before this returns. */
static inline int64_t fresh_free(void) {
    sigset_t one;
    sigemptyset(&one);
    sigaddset(&one, SIGUSR1);
    return pthread_sigmask(SIG_UNBLOCK, &one, NULL) == 0 ? 1 : 0;
}

/* To the process: the kernel hands it to a thread that does not hold it. */
static inline int64_t fresh_send(void) {
    return kill(getpid(), SIGUSR1) == 0 ? 1 : 0;
}

static _Atomic uintptr_t fresh_last = 0;

/* The handler: the thread first, then the count, so a worker that sees the
 * count sees the thread. */
static void fresh_on_signal(int sig) {
    (void)sig;
    atomic_store(&fresh_last, (uintptr_t)pthread_self());
    atomic_fetch_add(&fresh_heard, 1);
}

/* 1 where the handler last ran on the calling thread. */
static inline int64_t fresh_here(void) {
    return atomic_load(&fresh_last) == (uintptr_t)pthread_self() ? 1 : 0;
}

static inline int64_t fresh_heard_count(void) {
    return atomic_load(&fresh_heard);
}

/* The count once it reaches `want`, the thread yielding meanwhile; bounded at
 * ten million yields, seconds, so a signal that never comes ends the wait. */
static inline int64_t fresh_wait_heard(int64_t want) {
    for (int64_t tries = 0; atomic_load(&fresh_heard) < want && tries < 10000000; tries += 1) {
        sched_yield();
    }
    return atomic_load(&fresh_heard);
}

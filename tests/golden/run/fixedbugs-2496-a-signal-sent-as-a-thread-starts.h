/* Defect 2496: the program's own C for its case. It holds SIGUSR1 off the
 * calling thread alone, sends SIGUSR1 to the whole process, and counts the
 * handler's runs in a lock-free atomic, which a signal handler may touch. */
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

static inline void fresh_heard_one(void) {
    atomic_fetch_add(&fresh_heard, 1);
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

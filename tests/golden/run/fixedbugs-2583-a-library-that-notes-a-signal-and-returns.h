/* Defect 2583: the program's own C for its cases, a library that installs,
 * before `main`, a SIGTRAP, SIGILL and SIGABRT handler that only counts what
 * it got and returns, so the runtime's lease handler finds it and calls it
 * first; and the ways a program sends itself one. POSIX's signals and
 * `fork`, which a platform without them lacks: such a platform is told the
 * header is missing and skips the case (`absence.hero`). Nothing here
 * allocates or prints. */
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t noted_count = 0;

static void noted_handler(int sig) {
    (void)sig;
    noted_count++;
}

__attribute__((constructor)) static void noted_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = noted_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTRAP, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
}

/* How many signals the handler has noted. */
static inline int64_t noted(void) { return (int64_t)noted_count; }

/* SIGTRAP sent by the program to itself: 1 by `raise`, 2 by `kill` of its own
 * process, 3 by `pthread_kill` of its own thread. */
static inline int64_t noted_sent_self(int64_t how) {
    if (how == 1) raise(SIGTRAP);
    if (how == 2) kill(getpid(), SIGTRAP);
    if (how == 3) pthread_kill(pthread_self(), SIGTRAP);
    return (int64_t)noted_count;
}

/* SIGTRAP sent by another process: a child that sends it to its parent and
 * ends; the parent waits until the handler has noted it, at most 5 s. */
static inline int64_t noted_sent_by_a_child(void) {
    sig_atomic_t before = noted_count;
    pid_t parent = getpid();
    pid_t child = fork();
    if (child < 0) return -1;
    if (child == 0) {
        kill(parent, SIGTRAP);
        _exit(0);
    }
    for (int i = 0; i < 500 && noted_count == before; i++) usleep(10000);
    int status = 0;
    waitpid(child, &status, 0);
    return (int64_t)noted_count;
}

/* `abort`, which the handler notes and returns from. */
static inline void noted_abort(void) { abort(); }

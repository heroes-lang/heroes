/* Defect 2494: the program's own C for its case. Its handler is C and calls
 * `fork`, which runs the functions `pthread_atfork` holds before it forks,
 * inside the signal; the child leaves at once and the parent waits for it. */
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void forking_handler(int sig) {
    (void)sig;
    pid_t child = fork();
    if (child == 0) _exit(0);
    int status = 0;
    if (child > 0) (void)waitpid(child, &status, 0);
}

static inline int32_t forking_install(int32_t sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = forking_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t forking_raise(int32_t sig) { return raise(sig); }

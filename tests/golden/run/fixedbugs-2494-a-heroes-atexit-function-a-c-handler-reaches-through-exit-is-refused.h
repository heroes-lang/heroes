/* Defect 2494: the program's own C for its case. Its handler is C and ends
 * the program with `exit`, as many C programs' SIGTERM handlers do, and
 * `exit` runs the functions `atexit` holds, inside the signal. */
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void leaving_handler(int sig) {
    (void)sig;
    exit(0);
}

static inline int32_t leaving_install(int32_t sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = leaving_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t leaving_raise(int32_t sig) { return raise(sig); }

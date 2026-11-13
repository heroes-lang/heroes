/* Defect 2494: the program's own C for its case. It installs the function it
 * is handed as a signal handler through `sigaction`, so no installer a list
 * of names could hold (`signal`, `sigset`, `bsd_signal`) is ever called from
 * Heroes, and raises the signal on the calling thread. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static inline int32_t own_install(int32_t sig, void (*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t own_raise(int32_t sig) { return raise(sig); }

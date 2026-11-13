/* Defect 2494: the program's own C for its case. It installs the function it
 * is handed as an `SA_SIGINFO` handler, whose two pointers the Heroes
 * function takes as `ptr`, and raises the signal on the calling thread. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static inline int32_t info_install(int32_t sig, void (*handler)(int32_t, void *, void *)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = (void (*)(int, siginfo_t *, void *))handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t info_raise(int32_t sig) { return raise(sig); }

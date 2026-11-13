/* Defect 2494: the program's own C for its case. It holds a signal with
 * `sigset(SIG_HOLD)`, as POSIX's XSI `sigset` allows, raises it, and asks
 * whether it waits: 1, held and pending; the program ends with it so, and
 * the system drops it. */
#include <signal.h>
#include <stdint.h>
#include <unistd.h>

/* glibc declares `sigset` and `SIG_HOLD` for XSI alone, which a unit
 * compiled `-std=gnu11` is not, though its C library defines the function
 * (`sigset@@GLIBC_2.17`, Linux arm64's `nm`): they are written here as its
 * own `signal.h` and `bits/signum-generic.h` write them. Darwin's SDK declares
 * both. */
#if defined(__GLIBC__) && !defined(SIG_HOLD)
extern __sighandler_t sigset(int __sig, __sighandler_t __disp);
#define SIG_HOLD ((__sighandler_t)2)
#endif

static inline int32_t held_then_raised(int32_t sig) {
    sigset_t pending;
    if (sigset(sig, SIG_HOLD) == SIG_ERR) return -1;
    if (raise(sig) != 0) return -1;
    sigemptyset(&pending);
    if (sigpending(&pending) != 0) return -1;
    return sigismember(&pending, sig);
}

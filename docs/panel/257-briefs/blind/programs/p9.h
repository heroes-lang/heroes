/* a handler leaves by siglongjmp to the FIRST of 40 buffers saved in 40 live frames */
#include "prelude.h"

#define N 40
static sigjmp_buf envs[N];
static volatile int fired;
static void handler(int sig) { (void)sig; siglongjmp(envs[0], 1); }
__attribute__((noinline)) static int rec(int n) {
    if (sigsetjmp(envs[n], 1) == 0) {
        if (n + 1 < N) return rec(n + 1);
        if (!fired) { fired = 1; raise(SIGUSR1); }
        return -4;
    }
    return 0;
}
int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    if (rec(0) != 0 && !fired) return -3;
    return call(cb);
}

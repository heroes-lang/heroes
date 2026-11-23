/* fixedbugs-2704-handler.h: a signal's handler of C's that `siglongjmp`s out of the function
 * the program handed C (a watchdog's timeout), and then calls a second function of the
 * program's. The handler is C's and calls no Heroes function. */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

typedef int64_t (*wd_cb_t)(int64_t);

static sigjmp_buf wd_env;

static void wd_handler(int sig) {
    (void)sig;
    siglongjmp(wd_env, 1);
}

/* Raises SIGUSR1 on the calling thread, whose handler leaves for `wd_run`. */
static inline void wd_fire(void) { raise(SIGUSR1); }

/* Calls `work`, which never returns by itself (it fires the signal), and where
 * the handler jumped out of it, calls `after` and answers its answer. */
static inline int64_t wd_run(wd_cb_t work, wd_cb_t after) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = wd_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    if (sigsetjmp(wd_env, 1) == 0) {
        work(0);
        return -3;
    }
    return after(7);
}

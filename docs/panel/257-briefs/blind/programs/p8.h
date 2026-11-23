/* the handler saves a buffer of its own, jumps back to it by siglongjmp, then calls the program's function itself */
#include "prelude.h"

static cb_t f;
static int64_t r;
static sigjmp_buf hbuf;
static void handler(int sig) {
    (void)sig;
    if (sigsetjmp(hbuf, 1) == 0) siglongjmp(hbuf, 1);
    r = f(7);
}

int64_t run_case(cb_t cb) {
    f = cb;
    if (on(SIGUSR1, handler) != 0) return -2;
    raise(SIGUSR1);
    return r;
}

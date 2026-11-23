/* the handler itself calls the program's function */
#include "prelude.h"

static cb_t f;
static int64_t r;
static void handler(int sig) { (void)sig; r = f(7); }

int64_t run_case(cb_t cb) {
    f = cb;
    if (on(SIGUSR1, handler) != 0) return -2;
    raise(SIGUSR1);
    return r;
}

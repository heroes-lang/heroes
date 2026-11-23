/* the handler returns, then the program calls */
#include "prelude.h"

static volatile int hit;
static void handler(int sig) { (void)sig; hit = 1; }

int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    raise(SIGUSR1);
    if (!hit) return -3;
    return call(cb);
}

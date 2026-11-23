/* the handler switches context to where the program was before the signal, and is never resumed */
#include "prelude.h"

static ucontext_t before, handler_ctx;
static volatile int phase;
static void handler(int sig) { (void)sig; swapcontext(&handler_ctx, &before); }

int64_t run_case(cb_t cb) {
    if (on(SIGUSR2, handler) != 0) return -2;
    getcontext(&before);
    if (phase == 0) { phase = 1; raise(SIGUSR2); return -3; }
    return call(cb);
}

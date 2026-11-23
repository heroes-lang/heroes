/* the handler refills a buffer by memcpy from a save the main line made, then jumps to the refilled buffer */
#include "prelude.h"

static sigjmp_buf saved, copy;
static void handler(int sig) { (void)sig; memcpy(copy, saved, sizeof saved); siglongjmp(copy, 1); }

int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    if (sigsetjmp(saved, 1) == 0) { raise(SIGUSR1); return -3; }
    return call(cb);
}

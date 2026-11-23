/* a handler leaves by siglongjmp through a whole copy of the saved buffer */
#include "prelude.h"

static sigjmp_buf saved, copy;
static void handler(int sig) { (void)sig; siglongjmp(copy, 1); }

int64_t run_case(cb_t cb) {
    if (on(SIGUSR1, handler) != 0) return -2;
    if (sigsetjmp(saved, 1) == 0) {
        memcpy(copy, saved, sizeof saved);
        raise(SIGUSR1);
        return -3;
    }
    return call(cb);
}

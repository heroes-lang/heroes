/* a handler leaves by siglongjmp, the buffer in the heap */
#include "prelude.h"

static sigjmp_buf *env;
static void handler(int sig) { (void)sig; siglongjmp(*env, 1); }

int64_t run_case(cb_t cb) {
    env = malloc(sizeof *env);
    if (env == NULL || on(SIGUSR1, handler) != 0) return -2;
    if (sigsetjmp(*env, 1) == 0) { raise(SIGUSR1); return -3; }
    free(env);
    env = NULL;
    return call(cb);
}

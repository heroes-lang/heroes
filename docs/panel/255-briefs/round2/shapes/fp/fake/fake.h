/* Panel 255, the ffi-pragmatist (critic finding 8's Darwin twin): a correct
 * jump out of a handler through a buffer held by a frame that does not save
 * (a callee saves into it), which AddressSanitizer moves to its fake stack
 * where `detect_stack_use_after_return=1`. */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
static sigjmp_buf *fk_env;
static void fk_to(int sig) { (void)sig; siglongjmp(*fk_env, 1); }
__attribute__((noinline)) static int64_t fk_saver(sigjmp_buf *b, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = fk_to;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    fk_env = b;
    if (sigsetjmp(*b, 1) == 0) {
        raise(SIGUSR1);
        return -2;
    }
    return cb(5);
}
__attribute__((noinline)) static int64_t fk_run(int64_t (*cb)(int64_t)) {
    sigjmp_buf held;
    return fk_saver(&held, cb);
}

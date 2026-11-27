/* Defect 2522 (panel 251's R1): the program's own C for its twelve cases,
 * panel 249's depth programs. A handler of C's leaves by a jump OUT of
 * itself, back to where the program raised the signal, so no handler is
 * running any more; then the program hands C its function again, called with
 * no signal anywhere, from deeper than the abandoned handler (`depth_deep`, 32
 * frames of 512 bytes) and from shallower (`depth_shallow`). A route that
 * kept the abandoned handler's entry would refuse a correct program here, as
 * route B' alone did three of the twelve at `-O0` (panel 249's
 * compiler-engineer); route J drops it at the jump. How it jumps: 0
 * `siglongjmp`, 1 `longjmp`, 2 `_longjmp`; and on the thread's own stack or
 * on an alternate one (`SA_ONSTACK`). */
/* POSIX's, which a platform without POSIX's signals lacks: such a platform
 * is told the header is missing and skips the case (`absence.hero`). */
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

static sigjmp_buf depth_sig_env;
static jmp_buf depth_env;
static volatile int depth_how = 0;

static void depth_handler(int sig) {
    (void)sig;
    (void)__builtin_frame_address(0);
    if (depth_how == 0) siglongjmp(depth_sig_env, 1);
    if (depth_how == 1) longjmp(depth_env, 1);
    _longjmp(depth_env, 1);
}

static inline int32_t depth_install(int32_t sig, int32_t onstack) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = depth_handler;
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

/* 1 where the handler ran and left by its jump. */
static inline int32_t depth_escape(int32_t sig, int32_t how) {
    depth_how = how;
    if (how == 0) {
        if (sigsetjmp(depth_sig_env, 1) == 0) {
            raise(sig);
            return 0;
        }
        return 1;
    }
    if (setjmp(depth_env) == 0) {
        raise(sig);
        return 0;
    }
    return 1;
}

/* `n` frames of 512 bytes each below this one, then the function. */
static int64_t depth_down(int32_t n, int64_t (*cb)(int64_t)) {
    volatile char pad[512];
    pad[0] = (char)n;
    __asm__ volatile("" ::"r"(pad) : "memory"); /* kept whole at -O2 (step 199p) */
    if (n <= 0) return cb(7) + pad[0];
    return depth_down(n - 1, cb) + pad[0] - pad[0];
}

static inline int64_t depth_deep(int32_t n, int64_t (*cb)(int64_t)) { return depth_down(n, cb); }

static inline int64_t depth_shallow(int64_t (*cb)(int64_t)) { return cb(5); }

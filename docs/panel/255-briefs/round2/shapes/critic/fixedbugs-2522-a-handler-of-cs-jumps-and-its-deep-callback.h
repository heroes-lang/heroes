/* Defect 2522 (panel 251's R1): the program's own C for its cases, the shapes
 * the sitting's seats built (the compiler-engineer's `ij.c`, `ijv.c` and
 * `ijstale.c`). A handler of C's leaves a jump to a buffer and lands WITHIN
 * itself, then reaches the program's function through twelve frames of its
 * own, more than the thunk's walk climbs, so only a route that saw the
 * handler begin and not end can tell it is still running. How it jumps:
 *   0  `sigsetjmp` into its own buffer, a helper's `siglongjmp` back to it;
 *   1  `_setjmp` and `_longjmp` the same way;
 *   2  THE STALE RECORD: a buffer of the program's, filled once at a shallow
 *      frame that has returned (`jumps_prime`), refilled inside the handler
 *      by `memcpy` from the handler's own buffer, and jumped to: the jump
 *      stays inside the handler, while the save that last filled that buffer
 *      in the C library's sight was the shallow one. */
/* POSIX's, which a platform without POSIX's signals lacks: such a platform
 * is told the header is missing and skips the case (`absence.hero`). */
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

static int64_t (*jumps_cb)(int64_t) = NULL;
static volatile sig_atomic_t jumps_got = 0;
static volatile int jumps_how = 0;
static jmp_buf jumps_kept;

__attribute__((noinline)) static int64_t jumps_down(int n, int64_t v) {
    if (n == 0) return jumps_cb(v);
    int64_t r = jumps_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}

__attribute__((noinline)) static void jumps_back_sig(sigjmp_buf env) { siglongjmp(env, 1); }

__attribute__((noinline)) static void jumps_back_plain(jmp_buf env) { _longjmp(env, 1); }

__attribute__((noinline)) static void jumps_back_kept(void) { longjmp(jumps_kept, 1); }

__attribute__((noinline)) static int jumps_prime(void) {
    if (setjmp(jumps_kept) == 0) return 0;
    return 1;
}

static void jumps_handler(int sig) {
    if (jumps_how == 0) {
        sigjmp_buf here;
        if (sigsetjmp(here, 1) == 0) jumps_back_sig(here);
    } else if (jumps_how == 1) {
        jmp_buf here;
        if (_setjmp(here) == 0) jumps_back_plain(here);
    } else {
        jmp_buf here;
        if (setjmp(here) == 0) {
            memcpy(jumps_kept, here, sizeof here);
            jumps_back_kept();
        }
    }
    jumps_got = (sig_atomic_t)jumps_down(12, (int64_t)sig);
}

static inline int32_t jumps_install(int32_t sig, int32_t how, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    jumps_cb = cb;
    jumps_how = how;
    (void)jumps_prime();
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = jumps_handler;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

static inline int32_t jumps_raise(int32_t sig) { return raise(sig); }

static inline int64_t jumps_seen(void) { return (int64_t)jumps_got; }

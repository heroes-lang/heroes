/* Panel 255, the ffi-pragmatist: C2's known cost, measured. A coroutine on a
 * heap stack saves a buffer, raises a signal (its handler runs on the
 * coroutine's stack), the handler jumps out to that buffer (a correct jump
 * out), and the coroutine, the handler left, calls the program through twelve
 * frames, then returns to main. */
#define _XOPEN_SOURCE 700
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
static ucontext_t *cj_main, *cj_co;
static sigjmp_buf cj_env;
static int64_t (*cj_cb)(int64_t);
static volatile int64_t cj_got = -2;
static void cj_handler(int sig) { (void)sig; siglongjmp(cj_env, 1); }
__attribute__((noinline)) static int64_t cj_down(int n, int64_t v) {
    if (n == 0) return cj_cb(v);
    int64_t r = cj_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static void cj_body(void) {
    if (sigsetjmp(cj_env, 1) == 0) {
        raise(SIGUSR1);
    } else {
        cj_got = cj_down(12, 9);
    }
}
static inline int64_t cj_run(int64_t (*cb)(int64_t)) {
    size_t big = sizeof(ucontext_t) < 1024 ? 1024 : sizeof(ucontext_t);
    cj_cb = cb;
    cj_main = calloc(1, big);
    cj_co = calloc(1, big);
    void *stack = malloc((size_t)1 << 18);
    if (cj_main == NULL || cj_co == NULL || stack == NULL || getcontext(cj_co) != 0) return -1;
    cj_co->uc_stack.ss_sp = stack;
    cj_co->uc_stack.ss_size = (size_t)1 << 18;
    cj_co->uc_link = cj_main;
    makecontext(cj_co, cj_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = cj_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    swapcontext(cj_main, cj_co);
    return cj_got;
}

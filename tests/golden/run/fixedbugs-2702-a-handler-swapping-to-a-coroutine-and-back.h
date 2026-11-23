/* Panel 255, the ffi-pragmatist (critic finding 10): a handler that swaps to
 * a coroutine on a heap stack and back. how 1: the coroutine swaps back and
 * the handler, still running, calls the program through twelve frames (must
 * be refused; today's `swapcontext` reads `from` before the C library fills
 * it, so which words refuse it tells the cause apart). how 2: the coroutine
 * itself calls the program while the handler is suspended on the thread's
 * own stack. */
#define _XOPEN_SOURCE 700
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
static ucontext_t *c2_h, *c2_co;
static int64_t (*c2_cb)(int64_t);
static volatile int64_t c2_got = 0;
static int c2_how = 1;
__attribute__((noinline)) static int64_t c2_down(int n, int64_t v) {
    if (n == 0) return c2_cb(v);
    int64_t r = c2_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static void c2_body(void) {
    if (c2_how == 2) c2_got = c2_down(12, 2);
    swapcontext(c2_co, c2_h);
}
static void c2_handler(int sig) {
    (void)sig;
    swapcontext(c2_h, c2_co);
    if (c2_how == 1) c2_got = c2_down(12, 1);
}
static inline int64_t c2_run(int32_t how, int64_t (*cb)(int64_t)) {
    c2_how = how;
    c2_cb = cb;
    c2_h = calloc(1, sizeof(ucontext_t));
    c2_co = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 16);
    if (c2_h == NULL || c2_co == NULL || stack == NULL || getcontext(c2_co) != 0) return -1;
    c2_co->uc_stack.ss_sp = stack;
    c2_co->uc_stack.ss_size = (size_t)1 << 16;
    c2_co->uc_link = NULL;
    makecontext(c2_co, c2_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = c2_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    raise(SIGUSR1);
    return c2_got;
}

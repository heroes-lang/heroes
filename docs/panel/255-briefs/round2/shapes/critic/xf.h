/* Panel 255 critic, second reading, shape F: a STALE coroutine stack. A
 * helper makes a context on a 256 KiB local array and returns without ever
 * swapping to it (the range is left in any table of `makecontext` stacks);
 * then the plainest correct jump out of a handler, a static buffer saved in
 * a frame ABOVE that dead range, the handler's own frame landing INSIDE it,
 * and the program called 64 KiB deeper after the jump: MUST RUN. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
typedef int64_t (*xf_cb_t)(int64_t);
static xf_cb_t xf_cb;
static sigjmp_buf xf_env;
static void xf_nothing(void) {}
__attribute__((noinline)) static void xf_make(void) {
    _Alignas(16) char stack[(size_t)256 << 10];
    ucontext_t *uc = calloc(1, sizeof(ucontext_t));
    if (uc == NULL || getcontext(uc) != 0) return;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = sizeof stack;
    uc->uc_link = NULL;
    makecontext(uc, xf_nothing, 0);
    __asm__ volatile("" ::"r"(stack) : "memory");
}
__attribute__((noinline)) static void xf_pad_then_make(void) {
    volatile char pad[8192];
    pad[0] = 1;
    xf_make();
    (void)pad[0];
}
__attribute__((noinline)) static int64_t xf_deep(int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = xf_cb(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
static void xf_handler(int sig) {
    (void)sig;
    siglongjmp(xf_env, 1);
}
/* The signal raised 32 KiB below the saving frame, so the handler's frame
 * lies inside the dead range and the jump's target above it. */
__attribute__((noinline)) static void xf_raise_deep(void) {
    volatile char pad[32768];
    pad[0] = 1;
    raise(SIGUSR1);
    (void)pad[0];
}
__attribute__((noinline)) static int64_t xf_stale(xf_cb_t cb) {
    xf_cb = cb;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = xf_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    xf_pad_then_make();
    if (sigsetjmp(xf_env, 1) == 0) {
        xf_raise_deep();
        return -2;
    }
    return xf_deep(6);
}

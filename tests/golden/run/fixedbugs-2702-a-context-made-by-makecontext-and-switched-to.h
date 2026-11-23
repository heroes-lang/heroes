/* The compiler-engineer's third stack (p255/extra/extra.h, x_ctx and x_deep), copied unchanged but for the feature macros, so Darwin builds it. */
#ifndef XC_255_H
#define XC_255_H
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
typedef int64_t (*x_cb)(int64_t);
static __attribute__((noinline)) int64_t x_deep(x_cb cb, int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = cb(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
#define XC_SIZE (256u << 10)
static ucontext_t xc_h, xc_co, xc_co2;
static x_cb xc_cb;
static int xc_mode;
static volatile int64_t xc_got;
static void *xc_co_stack;
static void xc_fn(void) {
    if (xc_mode == 1) xc_got = xc_cb(7);
    if (xc_mode == 3) swapcontext(&xc_co2, &xc_h);
}
static void xc_handler(int sig) {
    (void)sig;
    getcontext(&xc_co);
    xc_co.uc_stack.ss_sp = xc_co_stack;
    xc_co.uc_stack.ss_size = XC_SIZE;
    xc_co.uc_link = &xc_h;
    makecontext(&xc_co, xc_fn, 0);
    swapcontext(&xc_h, &xc_co);
    if (xc_mode == 3) xc_got = xc_cb(9);
}
static inline int64_t x_ctx(int64_t mode, int64_t order, int64_t onstack, x_cb cb) {
    void *r1 = mmap(NULL, XC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    void *r2 = mmap(NULL, XC_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (r1 == MAP_FAILED || r2 == MAP_FAILED) return -1;
    void *lo = r1 < r2 ? r1 : r2, *hi = r1 < r2 ? r2 : r1;
    void *alt = order == 0 ? lo : hi;
    xc_co_stack = order == 0 ? hi : lo;
    xc_cb = cb;
    xc_mode = (int)mode;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = xc_handler;
    sigemptyset(&sa.sa_mask);
    if (onstack) {
        stack_t ss = {.ss_sp = alt, .ss_size = XC_SIZE, .ss_flags = 0};
        if (sigaltstack(&ss, NULL) != 0) return -2;
        sa.sa_flags = SA_ONSTACK;
    }
    if (sigaction(SIGUSR2, &sa, NULL) != 0) return -3;
    raise(SIGUSR2);
    if (mode == 2) return x_deep(cb, 1);
    return xc_got;
}
#endif

/* Panel 255, the completeness critic's second reading: shapes beside the two
 * seats' C3s, each a C program of the shape both seats already judged
 * (ctx2, the third stack m1 to m3, cojump), moved one step (CL-061).
 * Every function returns what the callback returned, or a negative number
 * where C itself failed. Built unchanged on Linux (glibc) and Darwin. */
#ifndef XR_255_H
#define XR_255_H
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <pthread.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*xr_cb)(int64_t);
static xr_cb xr_f;
static volatile int64_t xr_got = -7;
static int xr_how;

/* Twelve frames down before the callback, past the thunk's walk of eight. */
__attribute__((noinline)) static int64_t xr_down(int n, int64_t v) {
    if (n == 0) return xr_f(v);
    int64_t r = xr_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
/* The callback 64 KiB below the caller: deeper than a dead handler's entry. */
__attribute__((noinline)) static int64_t xr_deep(int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = xr_f(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
/* Whether `p` lies on the calling thread's own stack (printed to stderr, so a
 * run says where its coroutine's stack was: under ASan a local may not be). */
static void xr_where(const char *what, const void *p) {
    uintptr_t at = (uintptr_t)p, lo = 0, hi = 0;
#if defined(__APPLE__)
    hi = (uintptr_t)pthread_get_stackaddr_np(pthread_self());
    lo = hi - pthread_get_stacksize_np(pthread_self());
#else
    pthread_attr_t a;
    void *base = NULL;
    size_t size = 0;
    if (pthread_getattr_np(pthread_self(), &a) == 0) {
        pthread_attr_getstack(&a, &base, &size);
        pthread_attr_destroy(&a);
    }
    lo = (uintptr_t)base;
    hi = lo + size;
#endif
    fprintf(stderr, "where: %s %s the thread's own stack\n", what, at >= lo && at < hi ? "ON" : "OFF");
}
static void *xr_alt_install(void) {
    size_t size = (size_t)256 << 10;
    void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (alt == MAP_FAILED) return NULL;
    stack_t ss;
    ss.ss_sp = alt;
    ss.ss_size = size;
    ss.ss_flags = 0;
    if (sigaltstack(&ss, NULL) != 0) return NULL;
    return alt;
}
static int xr_on(int sig, void (*h)(int), int onstack) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    if (onstack && xr_alt_install() == NULL) return -1;
    return sigaction(sig, &sa, NULL);
}

/* SHAPE A: A COROUTINE WHOSE STACK IS AN ARRAY IN A LIVE FRAME OF THE
 * THREAD'S OWN STACK (the makecontext example's shape with its stacks as
 * locals), the handler swapping into it (on the alternate stack where
 * `onstack`). how 1: the coroutine calls the program while the handler is
 * suspended in swapcontext: MUST BE REFUSED (ctx2 how 2's rule). how 2: the
 * coroutine swaps back and the handler, still running, calls the program:
 * MUST BE REFUSED (ctx2 how 1's rule). */
static ucontext_t *xa_h, *xa_co;
static void xa_body(void) {
    if (xr_how == 1) xr_got = xr_down(12, 1);
    swapcontext(xa_co, xa_h);
}
static void xa_handler(int sig) {
    (void)sig;
    swapcontext(xa_h, xa_co);
    if (xr_how == 2) xr_got = xr_down(12, 2);
}
__attribute__((noinline)) static int64_t xr_coarray(int32_t how, int32_t onstack, xr_cb cb) {
    _Alignas(16) char stack[(size_t)1 << 16];
    xr_how = how;
    xr_f = cb;
    xa_h = calloc(1, sizeof(ucontext_t));
    xa_co = calloc(1, sizeof(ucontext_t));
    if (xa_h == NULL || xa_co == NULL || getcontext(xa_co) != 0) return -1;
    xr_where("the coroutine's stack is", stack);
    xa_co->uc_stack.ss_sp = stack;
    xa_co->uc_stack.ss_size = sizeof stack;
    xa_co->uc_link = NULL;
    makecontext(xa_co, xa_body, 0);
    if (xr_on(SIGUSR1, xa_handler, onstack) != 0) return -2;
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(stack) : "memory");
    return xr_got;
}

/* SHAPE B: A HANDLER LEFT BY setcontext, a correct escape: the runner's
 * context taken by getcontext, the handler (on the alternate stack where
 * `onstack`) resumes it, and the runner, the handler left, calls the program
 * 64 KiB deeper: MUST RUN (a jump out, as siglongjmp is). */
static ucontext_t *xb_esc;
static volatile int xb_phase;
static void xb_handler(int sig) {
    (void)sig;
    setcontext(xb_esc);
}
__attribute__((noinline)) static int64_t xr_escape(int32_t onstack, xr_cb cb) {
    xr_f = cb;
    xb_esc = calloc(1, sizeof(ucontext_t));
    if (xb_esc == NULL || xr_on(SIGUSR2, xb_handler, onstack) != 0) return -1;
    xb_phase = 0;
    if (getcontext(xb_esc) != 0) return -2;
    if (xb_phase == 0) {
        xb_phase = 1;
        raise(SIGUSR2);
        return -3;
    }
    return xr_deep(3);
}

/* SHAPE C: cojump (the ffi-pragmatist's) with its handler on the alternate
 * stack where `onstack`: a coroutine on a heap stack saves, raises, the
 * handler jumps OUT to that buffer, the coroutine then calls the program
 * twelve frames down: MUST RUN. */
static ucontext_t *xc_main, *xc_co;
static sigjmp_buf xc_env;
static void xc_handler(int sig) {
    (void)sig;
    siglongjmp(xc_env, 1);
}
static void xc_body(void) {
    if (sigsetjmp(xc_env, 1) == 0) {
        raise(SIGUSR1);
    } else {
        xr_got = xr_down(12, 9);
    }
}
static inline int64_t xr_cojump(int32_t onstack, xr_cb cb) {
    xr_f = cb;
    xr_got = -2;
    xc_main = calloc(1, sizeof(ucontext_t));
    xc_co = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 18);
    if (xc_main == NULL || xc_co == NULL || stack == NULL || getcontext(xc_co) != 0) return -1;
    xc_co->uc_stack.ss_sp = stack;
    xc_co->uc_stack.ss_size = (size_t)1 << 18;
    xc_co->uc_link = xc_main;
    makecontext(xc_co, xc_body, 0);
    if (xr_on(SIGUSR1, xc_handler, onstack) != 0) return -3;
    swapcontext(xc_main, xc_co);
    return xr_got;
}

/* SHAPE D: the plainest correct jump out, a static buffer, no save between,
 * then the callback 64 KiB deeper: MUST RUN (the twelve depth cases' shape;
 * the control a failed calibration must still follow). */
static sigjmp_buf xd_env;
static void xd_handler(int sig) {
    (void)sig;
    siglongjmp(xd_env, 1);
}
__attribute__((noinline)) static int64_t xr_static(int32_t onstack, xr_cb cb) {
    xr_f = cb;
    if (xr_on(SIGUSR2, xd_handler, onstack) != 0) return -1;
    if (sigsetjmp(xd_env, 1) == 0) {
        raise(SIGUSR2);
        return -2;
    }
    return xr_deep(4);
}

/* SHAPE E: the same through a HEAP buffer with a save between (x_heap):
 * MUST RUN; A refuses it, so it tells a decode from A. */
__attribute__((noinline)) static int xe_other(void) {
    sigjmp_buf o;
    return sigsetjmp(o, 0);
}
static sigjmp_buf *volatile xe_env;
static void xe_handler(int sig) {
    (void)sig;
    siglongjmp(*xe_env, 1);
}
__attribute__((noinline)) static int64_t xr_heap(xr_cb cb) {
    xr_f = cb;
    xe_env = malloc(sizeof(sigjmp_buf));
    if (xe_env == NULL || xr_on(SIGUSR1, xe_handler, 0) != 0) return -1;
    if (sigsetjmp(*xe_env, 1) == 0) {
        (void)xe_other();
        raise(SIGUSR1);
        return -2;
    }
    return xr_deep(5);
}
#endif

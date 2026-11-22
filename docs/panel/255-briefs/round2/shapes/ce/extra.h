/* Panel 255, compiler-engineer: the shapes beside the reproducers (CL-061),
 * each a correct C program unless its line says otherwise. Each returns what
 * the program's callback returned, or a negative number where C failed, and
 * calls the callback only where the line says it must run or be refused. */
#ifndef EXTRA_255_H
#define EXTRA_255_H
#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*x_cb)(int64_t);
static sigjmp_buf *volatile x_target;
static void x_leave(int sig) {
    (void)sig;
    siglongjmp(*x_target, 1);
}
static inline int x_install(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = x_leave;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    return sigaction(SIGUSR1, &sa, NULL);
}
/* A save of the program's own between a buffer's save and the jump to it. */
static __attribute__((noinline)) int x_other_save(void) {
    sigjmp_buf o;
    return sigsetjmp(o, 0);
}
/* The callback called from 64 KiB below the caller's frame, so DEEPER than
 * the dead handler's entry: the twelve depth programs' shape (panel 249), the
 * one a jump the runtime cannot follow leaves refused. */
static __attribute__((noinline)) int64_t x_deep(x_cb cb, int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = cb(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
/* Save into `b`, a save between where `between`, raise; the handler jumps
 * out through `b`; the callback then runs outside every handler, deeper than
 * the handler was: MUST RUN. */
static __attribute__((noinline)) int64_t x_through(sigjmp_buf *b, int between, x_cb cb) {
    x_target = b;
    if (x_install() != 0) return -1;
    if (sigsetjmp(*b, 1) == 0) {
        if (between) (void)x_other_save();
        raise(SIGUSR1);
        return -3;
    }
    return x_deep(cb, 1);
}

/* control: a static buffer. MUST RUN. */
static sigjmp_buf x_static_buf;
static inline int64_t x_static(x_cb cb) { return x_through(&x_static_buf, 1, cb); }

/* a heap buffer, no save between / a save between. MUST RUN. */
static inline int64_t x_heap0(x_cb cb) {
    sigjmp_buf *b = malloc(sizeof *b);
    int64_t r = x_through(b, 0, cb);
    free(b);
    return r;
}
static inline int64_t x_heap(x_cb cb) {
    sigjmp_buf *b = malloc(sizeof *b);
    int64_t r = x_through(b, 1, cb);
    free(b);
    return r;
}

/* thread-local storage, main thread. MUST RUN. */
static _Thread_local sigjmp_buf x_tls_buf;
static inline int64_t x_tls(x_cb cb) { return x_through(&x_tls_buf, 1, cb); }

/* a static buffer of a library loaded after main. MUST RUN. */
static inline int64_t x_dlopen(x_cb cb) {
    void *h = dlopen("./libxs.so", RTLD_NOW);
    if (h == NULL) return -10;
    sigjmp_buf *(*get)(void) = (sigjmp_buf * (*)(void)) dlsym(h, "xs_buf");
    if (get == NULL) return -11;
    return x_through(get(), 1, cb);
}

/* a static buffer of a library saved into, the library unloaded, then a
 * save. MUST RUN, the callback called at the end outside any handler. */
static inline int64_t x_dlclose(x_cb cb) {
    void *h = dlopen("./libxs.so", RTLD_NOW);
    if (h == NULL) return -10;
    sigjmp_buf *(*get)(void) = (sigjmp_buf * (*)(void)) dlsym(h, "xs_buf");
    if (get == NULL) return -11;
    (void)sigsetjmp(*get(), 0);
    dlclose(h);
    (void)x_other_save();
    return x_deep(cb, 1);
}

/* a page saved into, made PROT_NONE, a save, made readable again, then a
 * jump out of a handler through it. MUST RUN. */
static inline int64_t x_prot(x_cb cb) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    sigjmp_buf *volatile b = mmap(NULL, page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if ((void *)b == MAP_FAILED || x_install() != 0) return -1;
    x_target = b;
    if (sigsetjmp(*b, 1) == 0) {
        mprotect((void *)b, page, PROT_NONE);
        (void)x_other_save();
        mprotect((void *)b, page, PROT_READ | PROT_WRITE);
        raise(SIGUSR1);
        return -3;
    }
    int64_t r = x_deep(cb, 1);
    munmap((void *)b, page);
    return r;
}

/* a struct holding a buffer, moved by realloc after the save, a save, then a
 * jump out of a handler through the moved buffer. MUST RUN. */
typedef struct {
    int64_t tag;
    sigjmp_buf b;
} x_holder;
static inline int64_t x_realloc(x_cb cb) {
    x_holder *volatile s = malloc(sizeof(x_holder));
    if (s == NULL || x_install() != 0) return -1;
    if (sigsetjmp(s->b, 1) == 0) {
        x_holder *moved = realloc(s, 4u << 20);
        if (moved == NULL) return -2;
        s = moved;
        x_target = &s->b;
        (void)x_other_save();
        raise(SIGUSR1);
        return -3;
    }
    int64_t r = x_deep(cb, 1);
    free(s);
    return r;
}

/* a buffer saved at a shallow frame, refilled by memcpy OUTSIDE any handler
 * from a deeper live frame's save, and jumped to inside a handler: the jump
 * lands in the deeper frame, outside the handler. MUST RUN. */
static sigjmp_buf xr_B;
static __attribute__((noinline)) int64_t xr_deep(x_cb cb) {
    sigjmp_buf X;
    if (sigsetjmp(X, 1) == 0) {
        memcpy(xr_B, X, sizeof X);
        x_target = &xr_B;
        raise(SIGUSR1);
        return -3;
    }
    return x_deep(cb, 1);
}
static inline int64_t x_refill_out(x_cb cb) {
    if (x_install() != 0) return -1;
    (void)sigsetjmp(xr_B, 1);
    return xr_deep(cb);
}

/* a buffer held by a frame that does not save, saved into by a callee, a
 * save between, a jump out of a handler (critic 8). MUST RUN. */
static __attribute__((noinline)) int64_t xf_inner(sigjmp_buf *b, x_cb cb) { return x_through(b, 1, cb); }
static inline int64_t x_fake(x_cb cb) {
    sigjmp_buf held;
    return xf_inner(&held, cb);
}

/* THE THIRD STACK. A handler (on the alternate stack where `onstack`) makes a
 * context on a third stack and swaps into it. mode 1: the context calls the
 * callback, inside the signal: MUST BE REFUSED. mode 2: the context returns,
 * the handler returns, the callback runs after: MUST RUN. mode 3: the context
 * swaps back explicitly and the handler then calls the callback: MUST BE
 * REFUSED (the words tell the swapcontext pre-fill cause). order 0: the third
 * stack above the alternate one; order 1: below it. */
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
/* A REFILL BY A SAVE THE RUNTIME NEVER SEES, inside a handler: a static
 * buffer saved (seen) at this frame, a seen save between where `between`,
 * then inside the handler the buffer refilled by the C library's own
 * `__sigsetjmp` reached by `dlvsym` (as a library bound to it directly
 * would), a jump to it WITHIN the handler, and the callback from the still
 * running handler: MUST BE REFUSED. */
#if defined(__aarch64__)
#define XU_VERSION "GLIBC_2.17"
#else
#define XU_VERSION "GLIBC_2.2.5"
#endif
static sigjmp_buf xu_B;
typedef int (*xu_fn)(struct __jmp_buf_tag *, int);
static xu_fn xu_real;
static x_cb xu_cb;
static volatile int64_t xu_got = -7;
static __attribute__((noinline)) void xu_jump(void) { siglongjmp(xu_B, 1); }
/* Twelve frames down before the callback, past the walk's eight (ijstale's). */
static __attribute__((noinline)) int64_t x_down(int n, x_cb cb, int64_t v) {
    if (n == 0) return cb(v);
    int64_t r = x_down(n - 1, cb, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static __attribute__((noinline)) void xu_handler(int sig) {
    (void)sig;
    if (xu_real(xu_B, 0) == 0) xu_jump();
    xu_got = x_down(12, xu_cb, 5);
}
static inline int64_t x_unseen_run(int between, x_cb cb) {
    xu_real = (xu_fn)dlvsym(RTLD_NEXT, "__sigsetjmp", XU_VERSION);
    if (xu_real == NULL) return -20;
    xu_cb = cb;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = xu_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    (void)sigsetjmp(xu_B, 0);
    if (between) (void)x_other_save();
    raise(SIGUSR1);
    return xu_got;
}
static inline int64_t x_unseen0(x_cb cb) { return x_unseen_run(0, cb); }
static inline int64_t x_unseen(x_cb cb) { return x_unseen_run(1, cb); }
/* A PARTIAL COPY: a static buffer saved (seen) at this frame, a seen save
 * between, then inside the handler a save of the handler's own (seen) of
 * which ONLY `__jmpbuf`, the registers, is copied into the static buffer, a
 * jump to it WITHIN the handler, the callback twelve frames down: MUST BE
 * REFUSED. (On x86-64 `__jmpbuf` is 64 bytes and holds no byte past 64; on
 * arm64 it is 176.) */
static sigjmp_buf xp_B;
static x_cb xp_cb;
static volatile int64_t xp_got = -7;
static __attribute__((noinline)) void xp_jump(void) { siglongjmp(xp_B, 1); }
static __attribute__((noinline)) void xp_handler(int sig) {
    (void)sig;
    sigjmp_buf here;
    if (sigsetjmp(here, 0) == 0) {
        memcpy(xp_B[0].__jmpbuf, here[0].__jmpbuf, sizeof(__jmp_buf));
        xp_jump();
    }
    xp_got = x_down(12, xp_cb, 6);
}
static inline int64_t x_partial(x_cb cb) {
    xp_cb = cb;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = xp_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    (void)sigsetjmp(xp_B, 0);
    (void)x_other_save();
    raise(SIGUSR1);
    return xp_got;
}
#endif

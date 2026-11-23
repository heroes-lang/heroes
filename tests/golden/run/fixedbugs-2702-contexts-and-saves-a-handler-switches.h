/* Panel 255, round two, the ffi-pragmatist on Darwin: the shapes C4's own
 * rules make decisive (round2.md, critic-round2-brief.md finding 7), written
 * here, plus two of the seat's own beside them. Every function returns what
 * the callback returned, or a negative number where C itself failed. Each
 * MUST is stated on its shape. Builds on Darwin (`_XOPEN_SOURCE` for the
 * context functions). */
#ifndef R2NEW_255_H
#define R2NEW_255_H
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

typedef int64_t (*nr_cb)(int64_t);
static nr_cb nr_f;
static volatile int64_t nr_got = -7;
static volatile int nr_how;

__attribute__((noinline)) static int64_t nr_down(int n, int64_t v) {
    if (n == 0) return nr_f(v);
    int64_t r = nr_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
__attribute__((noinline)) static int64_t nr_deep(int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = nr_f(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
static void *nr_alt_install(void) {
    size_t size = (size_t)256 << 10;
    void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (alt == MAP_FAILED) return NULL;
    stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
    if (sigaltstack(&ss, NULL) != 0) return NULL;
    return alt;
}
static int nr_on(int sig, void (*h)(int), int onstack) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    if (onstack && nr_alt_install() == NULL) return -1;
    return sigaction(sig, &sa, NULL);
}

/* A-PRIME: shape A spelled getcontext + setcontext, the coroutine's uc_link
 * the handler's context. The handler saves its own context, setcontexts into
 * the coroutine; how 1: the coroutine calls the program; how 2: the coroutine
 * returns, the C library resumes the handler through uc_link, and the handler
 * calls it. onstack 0: the coroutine's stack an array in a live frame of the
 * thread's own stack, the handler on the thread's own; onstack 1: a heap
 * stack, the handler on the alternate stack. MUST BE REFUSED, every form. */
static ucontext_t *ap_h, *ap_co;
static volatile int ap_phase;
static void ap_body(void) {
    if (nr_how == 1) nr_got = nr_down(12, 11);
}
static void ap_handler(int sig) {
    (void)sig;
    ap_phase = 0;
    getcontext(ap_h);
    if (ap_phase == 0) {
        ap_phase = 1;
        setcontext(ap_co);
    }
    if (nr_how == 2) nr_got = nr_down(12, 12);
}
__attribute__((noinline)) static int64_t nr_aprime(int32_t how, int32_t onstack, nr_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    nr_how = how;
    nr_f = cb;
    ap_h = calloc(1, sizeof(ucontext_t));
    ap_co = calloc(1, sizeof(ucontext_t));
    void *stack = onstack ? malloc((size_t)1 << 18) : (void *)local;
    size_t size = onstack ? (size_t)1 << 18 : sizeof local;
    if (ap_h == NULL || ap_co == NULL || stack == NULL || getcontext(ap_co) != 0) return -1;
    ap_co->uc_stack.ss_sp = stack;
    ap_co->uc_stack.ss_size = size;
    ap_co->uc_link = ap_h;
    makecontext(ap_co, ap_body, 0);
    if (nr_on(SIGUSR1, ap_handler, onstack) != 0) return -2;
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    return nr_got;
}

/* A BY AN UNSEEN SWITCH: shape A with the switch written in assembly, as
 * Boost.Context or libco write it, so no context function is called: the
 * coroutine's stack an array in a live frame of the thread's own stack, the
 * handler on the thread's own. how 1: the coroutine calls the program while
 * the handler is switched away from; how 2: the coroutine switches back and
 * the handler calls it. MUST BE REFUSED. */
__attribute__((naked)) static void au_switch(void **from_sp, void *to_sp) {
    __asm__ volatile("sub sp, sp, #176\n\t"
                     "stp x19, x20, [sp, #0]\n\t"
                     "stp x21, x22, [sp, #16]\n\t"
                     "stp x23, x24, [sp, #32]\n\t"
                     "stp x25, x26, [sp, #48]\n\t"
                     "stp x27, x28, [sp, #64]\n\t"
                     "stp x29, x30, [sp, #80]\n\t"
                     "stp d8, d9, [sp, #96]\n\t"
                     "stp d10, d11, [sp, #112]\n\t"
                     "stp d12, d13, [sp, #128]\n\t"
                     "stp d14, d15, [sp, #144]\n\t"
                     "mov x9, sp\n\t"
                     "str x9, [x0]\n\t"
                     "mov sp, x1\n\t"
                     "ldp x19, x20, [sp, #0]\n\t"
                     "ldp x21, x22, [sp, #16]\n\t"
                     "ldp x23, x24, [sp, #32]\n\t"
                     "ldp x25, x26, [sp, #48]\n\t"
                     "ldp x27, x28, [sp, #64]\n\t"
                     "ldp x29, x30, [sp, #80]\n\t"
                     "ldp d8, d9, [sp, #96]\n\t"
                     "ldp d10, d11, [sp, #112]\n\t"
                     "ldp d12, d13, [sp, #128]\n\t"
                     "ldp d14, d15, [sp, #144]\n\t"
                     "add sp, sp, #176\n\t"
                     "ret");
}
static void *au_h_sp, *au_co_sp;
__attribute__((noreturn)) static void au_entry(void) {
    if (nr_how == 1) nr_got = nr_down(12, 21);
    au_switch(&au_co_sp, au_h_sp);
    __builtin_trap();
}
static void au_handler(int sig) {
    (void)sig;
    au_switch(&au_h_sp, au_co_sp);
    if (nr_how == 2) nr_got = nr_down(12, 22);
}
__attribute__((noinline)) static int64_t nr_unseen_switch(int32_t how, nr_cb cb) {
    _Alignas(16) char stack[(size_t)1 << 16];
    nr_how = how;
    nr_f = cb;
    uintptr_t top = ((uintptr_t)stack + sizeof stack) & ~(uintptr_t)15;
    uint64_t *frame = (uint64_t *)(top - 176);
    memset(frame, 0, 176);
    frame[10] = 0;                        /* x29 */
    frame[11] = (uint64_t)(uintptr_t)au_entry; /* x30: where the first switch lands */
    au_co_sp = frame;
    if (nr_on(SIGUSR1, au_handler, 0) != 0) return -2;
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(stack) : "memory");
    return nr_got;
}

/* N: NESTED HANDLERS SUSPENDED AND RESUMED OUT OF ORDER. H1 (SIGUSR1, own
 * stack) swaps to K1; K1 raises SIGUSR2, whose H2 runs on K1 and swaps to K2;
 * K2 resumes H1, which returns; the runner, the first signal over, swaps into
 * K2, which calls the program while H2 has never returned. MUST BE REFUSED. */
static ucontext_t *n_h1, *n_k1, *n_h2, *n_k2, *n_k2back, *n_main;
static void n_k1_body(void) {
    raise(SIGUSR2);
    swapcontext(n_k1, n_main);
}
static void n_k2_body(void) {
    swapcontext(n_k2back, n_h1);
    nr_got = nr_down(12, 31);
    setcontext(n_main);
}
static void n_handler1(int sig) {
    (void)sig;
    swapcontext(n_h1, n_k1);
}
static void n_handler2(int sig) {
    (void)sig;
    swapcontext(n_h2, n_k2);
}
static ucontext_t *n_make(void (*fn)(void), ucontext_t *link) {
    ucontext_t *uc = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 18);
    if (uc == NULL || stack == NULL || getcontext(uc) != 0) return NULL;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = (size_t)1 << 18;
    uc->uc_link = link;
    makecontext(uc, fn, 0);
    return uc;
}
__attribute__((noinline)) static int64_t nr_nested(nr_cb cb) {
    nr_f = cb;
    n_h1 = calloc(1, sizeof(ucontext_t));
    n_h2 = calloc(1, sizeof(ucontext_t));
    n_k2back = calloc(1, sizeof(ucontext_t));
    n_main = calloc(1, sizeof(ucontext_t));
    n_k1 = n_make(n_k1_body, NULL);
    n_k2 = n_make(n_k2_body, NULL);
    if (n_h1 == NULL || n_h2 == NULL || n_k2back == NULL || n_main == NULL || n_k1 == NULL || n_k2 == NULL) return -1;
    if (nr_on(SIGUSR1, n_handler1, 0) != 0 || nr_on(SIGUSR2, n_handler2, 0) != 0) return -2;
    raise(SIGUSR1);
    swapcontext(n_main, n_k2back);
    return nr_got;
}

/* F-PRIME: shape f (xf.h) with its context swapped into once before the
 * helper returns, so a table learned at a swap holds the range too; then the
 * plainest correct jump out of a handler whose frame lies inside the dead
 * range. MUST RUN. */
static sigjmp_buf fp_env;
static ucontext_t *fp_uc, *fp_back;
static void fp_body(void) { swapcontext(fp_uc, fp_back); }
__attribute__((noinline)) static void fp_make(void) {
    _Alignas(16) char stack[(size_t)256 << 10];
    fp_uc = calloc(1, sizeof(ucontext_t));
    fp_back = calloc(1, sizeof(ucontext_t));
    if (fp_uc == NULL || fp_back == NULL || getcontext(fp_uc) != 0) return;
    fp_uc->uc_stack.ss_sp = stack;
    fp_uc->uc_stack.ss_size = sizeof stack;
    fp_uc->uc_link = NULL;
    makecontext(fp_uc, fp_body, 0);
    swapcontext(fp_back, fp_uc);
    __asm__ volatile("" ::"r"(stack) : "memory");
}
__attribute__((noinline)) static void fp_pad_then_make(void) {
    volatile char pad[8192];
    pad[0] = 1;
    fp_make();
    (void)pad[0];
}
static void fp_handler(int sig) {
    (void)sig;
    siglongjmp(fp_env, 1);
}
__attribute__((noinline)) static void fp_raise_deep(void) {
    volatile char pad[32768];
    pad[0] = 1;
    raise(SIGUSR1);
    (void)pad[0];
}
__attribute__((noinline)) static int64_t nr_fprime(nr_cb cb) {
    nr_f = cb;
    if (nr_on(SIGUSR1, fp_handler, 0) != 0) return -1;
    fp_pad_then_make();
    if (sigsetjmp(fp_env, 1) == 0) {
        fp_raise_deep();
        return -2;
    }
    return nr_deep(41);
}

/* F-SECOND: a coroutine's mmap'ed stack run once, unmapped, and the same range
 * mapped again and registered as the alternate stack; then d_1 (a static
 * buffer, the handler on the alternate stack, siglongjmp out, the program
 * called 64 KiB deeper). MUST RUN. */
static sigjmp_buf fpp_env;
static ucontext_t *fpp_uc, *fpp_back;
static void fpp_body(void) { swapcontext(fpp_uc, fpp_back); }
static void fpp_handler(int sig) {
    (void)sig;
    siglongjmp(fpp_env, 1);
}
__attribute__((noinline)) static int64_t nr_fsecond(nr_cb cb) {
    nr_f = cb;
    size_t size = (size_t)256 << 10;
    void *r = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    fpp_uc = calloc(1, sizeof(ucontext_t));
    fpp_back = calloc(1, sizeof(ucontext_t));
    if (r == MAP_FAILED || fpp_uc == NULL || fpp_back == NULL || getcontext(fpp_uc) != 0) return -1;
    fpp_uc->uc_stack.ss_sp = r;
    fpp_uc->uc_stack.ss_size = size;
    fpp_uc->uc_link = NULL;
    makecontext(fpp_uc, fpp_body, 0);
    swapcontext(fpp_back, fpp_uc);
    munmap(r, size);
    void *again = mmap(r, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if (again != r) return -2;
    stack_t ss = {.ss_sp = again, .ss_size = size, .ss_flags = 0};
    if (sigaltstack(&ss, NULL) != 0) return -3;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = fpp_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    if (sigaction(SIGUSR2, &sa, NULL) != 0) return -4;
    if (sigsetjmp(fpp_env, 1) == 0) {
        raise(SIGUSR2);
        return -5;
    }
    return nr_deep(42);
}

/* F-THIRD (the seat's own, beside f): the dead range carved from the thread's
 * own stack holds the jump's TARGET, and the handler's frame lies below it. A
 * helper makes a context on a 256 KiB local array and returns; a frame of the
 * same parent saves a static buffer from inside that dead range, then raises
 * 320 KiB below it; the handler jumps out to the buffer, and the program is
 * called 64 KiB deeper. A correct program: MUST RUN. */
static sigjmp_buf ft_env;
static void ft_nothing(void) {}
__attribute__((noinline)) static void ft_make(void) {
    _Alignas(16) char stack[(size_t)256 << 10];
    ucontext_t *uc = calloc(1, sizeof(ucontext_t));
    if (uc == NULL || getcontext(uc) != 0) return;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = sizeof stack;
    uc->uc_link = NULL;
    makecontext(uc, ft_nothing, 0);
    __asm__ volatile("" ::"r"(stack) : "memory");
}
static void ft_handler(int sig) {
    (void)sig;
    siglongjmp(ft_env, 1);
}
__attribute__((noinline)) static void ft_raise_deeper(void) {
    volatile char pad[(size_t)320 << 10];
    pad[0] = 1;
    raise(SIGUSR1);
    (void)pad[0];
}
__attribute__((noinline)) static int64_t ft_inside(void) {
    volatile char pad[(size_t)64 << 10];
    pad[0] = 1;
    if (sigsetjmp(ft_env, 1) == 0) {
        ft_raise_deeper();
        return -2;
    }
    int64_t r = nr_deep(43);
    (void)pad[0];
    return r;
}
__attribute__((noinline)) static int64_t nr_fthird(nr_cb cb) {
    nr_f = cb;
    if (nr_on(SIGUSR1, ft_handler, 0) != 0) return -1;
    ft_make();
    return ft_inside();
}

/* A-SECOND (the seat's own): shape A spelled with siglongjmp. A coroutine,
 * started by the runner, saves a buffer and swaps back; the signal comes; the
 * handler jumps INTO the coroutine through that buffer.
 * save 1 (handler on the alternate stack, the coroutine on the heap): the
 * handler first saves its own context; the coroutine, jumped into, resumes
 * that context, and the handler, running again, calls the program. MUST BE
 * REFUSED.
 * save 0 (handler on the thread's own stack, the coroutine's stack an array
 * in a live frame of it): the handler saves nothing; the coroutine, jumped
 * into, calls the program and returns to the runner through uc_link. Correct
 * by the reasoning that makes c_* MUST RUN (the handler is left for good). */
static ucontext_t *as_co, *as_main, *as_hc;
static sigjmp_buf as_kb;
static volatile int as_phase, as_save;
static void as_body(void) {
    if (sigsetjmp(as_kb, 0) == 0) {
        swapcontext(as_co, as_main);
    }
    if (as_save) {
        setcontext(as_hc);
    } else {
        nr_got = nr_down(12, 51);
    }
}
static void as_handler(int sig) {
    (void)sig;
    as_phase = 0;
    if (as_save) getcontext(as_hc);
    if (as_phase == 0) {
        as_phase = 1;
        siglongjmp(as_kb, 1);
    }
    nr_got = nr_down(12, 52);
}
__attribute__((noinline)) static int64_t nr_asecond(int32_t save, nr_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    nr_f = cb;
    as_save = save;
    as_co = calloc(1, sizeof(ucontext_t));
    as_main = calloc(1, sizeof(ucontext_t));
    as_hc = calloc(1, sizeof(ucontext_t));
    ucontext_t *done = calloc(1, sizeof(ucontext_t));
    void *stack = save ? malloc((size_t)1 << 18) : (void *)local;
    size_t size = save ? (size_t)1 << 18 : sizeof local;
    if (as_co == NULL || as_main == NULL || as_hc == NULL || done == NULL || stack == NULL || getcontext(as_co) != 0) {
        return -1;
    }
    as_co->uc_stack.ss_sp = stack;
    as_co->uc_stack.ss_size = size;
    as_co->uc_link = done;
    makecontext(as_co, as_body, 0);
    if (nr_on(SIGUSR1, as_handler, save) != 0) return -2;
    swapcontext(as_main, as_co);
    volatile int finished = 0;
    getcontext(done);
    if (!finished) {
        finished = 1;
        raise(SIGUSR1);
    }
    __asm__ volatile("" ::"r"(local) : "memory");
    return nr_got;
}

/* G: A HANDLER SWAPPED AWAY FROM AND NEVER RESUMED, three forms. Each is
 * correct by the reasoning that makes b_* MUST RUN (the handler is left for
 * good); its verdict is the author's question (round2.md).
 * form 1: the handler escapes by swapcontext to a context the runner took
 * before the signal (b spelled with swapcontext); onstack 0 or 1.
 * form 2: the handler swaps to a coroutine, which siglongjmps to a buffer the
 * runner saved before the signal.
 * form 3: the handler swaps to a coroutine, which returns through uc_link to a
 * context the runner took before the signal.
 * The runner then calls the program 64 KiB deeper. */
static ucontext_t *g_esc, *g_tmp, *g_co;
static sigjmp_buf g_buf;
static volatile int g_phase, g_form;
static void g_body(void) {
    if (g_form == 2) siglongjmp(g_buf, 1);
}
static void g_handler(int sig) {
    (void)sig;
    if (g_form == 1) {
        swapcontext(g_tmp, g_esc);
    } else {
        swapcontext(g_tmp, g_co);
    }
}
__attribute__((noinline)) static int64_t nr_g(int32_t form, int32_t onstack, nr_cb cb) {
    nr_f = cb;
    g_form = form;
    g_esc = calloc(1, sizeof(ucontext_t));
    g_tmp = calloc(1, sizeof(ucontext_t));
    g_co = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 18);
    if (g_esc == NULL || g_tmp == NULL || g_co == NULL || stack == NULL || getcontext(g_co) != 0) return -1;
    g_co->uc_stack.ss_sp = stack;
    g_co->uc_stack.ss_size = (size_t)1 << 18;
    g_co->uc_link = g_esc;
    makecontext(g_co, g_body, 0);
    if (nr_on(SIGUSR2, g_handler, onstack) != 0) return -2;
    g_phase = 0;
    if (form == 2) {
        if (sigsetjmp(g_buf, 1) != 0) return nr_deep(60 + form);
    }
    getcontext(g_esc);
    if (g_phase == 0) {
        g_phase = 1;
        raise(SIGUSR2);
        return -3;
    }
    return nr_deep(60 + form);
}
#endif

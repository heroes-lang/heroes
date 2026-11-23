/* Panel 255, third reading, the critic: shapes beside the two seats' round-two
 * shapes, written to decide the rows where their C4s differ. Portable: Darwin
 * (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`). Every function returns what the
 * callback returned, or a negative number where C itself failed. Each MUST is
 * stated on its shape. */
#ifndef CR_255_H
#define CR_255_H
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*cr_cb)(int64_t);
static cr_cb cr_f;
static volatile int64_t cr_got = -7;
static volatile int cr_how;

__attribute__((noinline)) static int64_t cr_down(int n, int64_t v) {
    if (n == 0) return cr_f(v);
    int64_t r = cr_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static int cr_on(int sig, void (*h)(int), int onstack) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    if (onstack) {
        size_t size = (size_t)256 << 10;
        void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (alt == MAP_FAILED) return -1;
        stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
        if (sigaltstack(&ss, NULL) != 0) return -1;
    }
    return sigaction(sig, &sa, NULL);
}

/* GR: G RESUMED. The handler escapes by swapcontext to a context the runner
 * took before the signal (G's form 1). The signal is raised 16 KiB below the
 * runner, so nothing the runner does next writes over the handler's frames.
 * The runner then makes ONE seen move 64 KiB below them, through an alloca it
 * never writes but at its bottom: variant 0 a sigsetjmp/siglongjmp pair on a
 * static buffer (the thread seen at a jump's target), variant 1 the program
 * called (G's own callback, the author's question), variant 2 nothing (the
 * control: no move below). Then the runner resumes the handler's own swap by
 * setcontext; the handler, running again inside its signal, calls the
 * program, then leaves by siglongjmp. That last call is made from inside a
 * running handler: MUST BE REFUSED, every variant and orientation. */
static ucontext_t *gr_h, *gr_pre;
static sigjmp_buf gr_out, gr_b;
static volatile int gr_phase, gr_variant;
static void gr_handler(int sig) {
    (void)sig;
    swapcontext(gr_h, gr_pre);
    cr_got = cr_down(12, 70 + gr_variant);
    siglongjmp(gr_out, 1);
}
__attribute__((noinline)) static void gr_raise_deep(void) {
    volatile char pad[16384];
    pad[0] = 1;
    raise(SIGUSR1);
    (void)pad[0];
}
__attribute__((noinline)) static void gr_deep(void) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    if (gr_variant == 0) {
        if (sigsetjmp(gr_b, 0) == 0) siglongjmp(gr_b, 1);
    } else if (gr_variant == 1) {
        int64_t r = cr_down(2, 69);
        (void)r;
    }
    __asm__ volatile("" ::"r"(pad) : "memory");
}
__attribute__((noinline)) static int64_t gr_run(int32_t variant, int32_t onstack, cr_cb cb) {
    cr_f = cb;
    gr_variant = variant;
    gr_h = calloc(1, sizeof(ucontext_t));
    gr_pre = calloc(1, sizeof(ucontext_t));
    if (gr_h == NULL || gr_pre == NULL) return -1;
    if (cr_on(SIGUSR1, gr_handler, onstack) != 0) return -2;
    if (sigsetjmp(gr_out, 1) != 0) {
        free(gr_h);
        free(gr_pre);
        return cr_got;
    }
    gr_phase = 0;
    getcontext(gr_pre);
    if (gr_phase == 0) {
        gr_phase = 1;
        gr_raise_deep();
        return -3;
    }
    if (gr_phase == 1) {
        gr_phase = 2;
        gr_deep();
        setcontext(gr_h);
        return -4;
    }
    return -5;
}

/* CV: A-PRIME SPLIT INTO ITS TWO DIFFERENCES FROM THE ENGINEER'S apu_*. The
 * handler saves its own context (seen: getcontext; unseen: the C library's
 * getcontext reached by dlvsym on glibc, by dlsym(RTLD_NEXT) on Darwin), then
 * setcontext()s into a coroutine whose uc_link is that context; how 1 the
 * coroutine calls the program, how 2 it returns through uc_link and the
 * handler calls it. The coroutine's stack: 0 an array in the runner's live
 * frame on the thread's own stack, 1 a static array, 2 the heap. The handler
 * on the alternate stack where `onstack`. MUST BE REFUSED, every combination. */
typedef int (*cv_get_fn)(ucontext_t *);
static cv_get_fn cv_get;
static ucontext_t *cv_hc, *cv_co;
static volatile int cv_back;
_Alignas(16) static char cv_static[(size_t)1 << 16];
static void cv_body(void) {
    if (cr_how == 1) cr_got = cr_down(12, 31);
}
static void cv_handler(int sig) {
    (void)sig;
    cv_back = 0;
    cv_get(cv_hc);
    if (cv_back == 0) {
        cv_back = 1;
        setcontext(cv_co);
    }
    if (cr_how == 2) cr_got = cr_down(12, 32);
}
__attribute__((noinline)) static int64_t cv_run(int32_t how, int32_t onstack, int32_t stack, int32_t unseen, cr_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    if (unseen) {
#if defined(__APPLE__)
        cv_get = (cv_get_fn)dlsym(RTLD_NEXT, "getcontext");
#elif defined(__aarch64__)
        cv_get = (cv_get_fn)dlvsym(RTLD_NEXT, "getcontext", "GLIBC_2.17");
#else
        cv_get = (cv_get_fn)dlvsym(RTLD_NEXT, "getcontext", "GLIBC_2.2.5");
#endif
    } else {
        cv_get = getcontext;
    }
    if (cv_get == NULL) return -9;
    cr_how = how;
    cr_f = cb;
    cr_got = -7;
    cv_hc = calloc(1, sizeof(ucontext_t));
    cv_co = calloc(1, sizeof(ucontext_t));
    void *heap = stack == 2 ? malloc((size_t)1 << 18) : NULL;
    if (cv_hc == NULL || cv_co == NULL || (stack == 2 && heap == NULL) || getcontext(cv_co) != 0) return -1;
    cv_co->uc_stack.ss_sp = stack == 0 ? (void *)local : stack == 1 ? (void *)cv_static : heap;
    cv_co->uc_stack.ss_size = stack == 2 ? (size_t)1 << 18 : (size_t)1 << 16;
    cv_co->uc_link = cv_hc;
    makecontext(cv_co, cv_body, 0);
    if (cr_on(SIGUSR1, cv_handler, onstack) != 0) return -2;
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    int64_t got = cr_got;
    free(cv_hc);
    free(cv_co);
    free(heap);
    return got;
}


/* AX: THE DARWIN SEAT'S A-SECOND (`as_1`) IN EVERY ORIENTATION, its save seen
 * or unseen (cv_get as CV's). A coroutine, started by the runner, saves a
 * buffer and swaps back; the signal comes; the handler saves its own context,
 * then siglongjmp()s INTO the coroutine through that buffer; the coroutine
 * resumes the handler's context; the handler, running again, calls the
 * program and returns. Stack 0 an array in the runner's live frame, 2 the
 * heap; the handler on the alternate stack where `onstack`. MUST BE REFUSED,
 * every combination. */
static ucontext_t *ax_co, *ax_main, *ax_hc;
static sigjmp_buf ax_kb;
static volatile int ax_phase;
static void ax_body(void) {
    if (sigsetjmp(ax_kb, 0) == 0) swapcontext(ax_co, ax_main);
    setcontext(ax_hc);
}
static void ax_handler(int sig) {
    (void)sig;
    ax_phase = 0;
    cv_get(ax_hc);
    if (ax_phase == 0) {
        ax_phase = 1;
        siglongjmp(ax_kb, 1);
    }
    cr_got = cr_down(12, 53);
}
__attribute__((noinline)) static int64_t ax_run(int32_t onstack, int32_t stack, int32_t unseen, cr_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    if (unseen) {
#if defined(__APPLE__)
        cv_get = (cv_get_fn)dlsym(RTLD_NEXT, "getcontext");
#elif defined(__aarch64__)
        cv_get = (cv_get_fn)dlvsym(RTLD_NEXT, "getcontext", "GLIBC_2.17");
#else
        cv_get = (cv_get_fn)dlvsym(RTLD_NEXT, "getcontext", "GLIBC_2.2.5");
#endif
    } else {
        cv_get = getcontext;
    }
    if (cv_get == NULL) return -9;
    cr_f = cb;
    cr_got = -7;
    ax_co = calloc(1, sizeof(ucontext_t));
    ax_main = calloc(1, sizeof(ucontext_t));
    ax_hc = calloc(1, sizeof(ucontext_t));
    void *heap = stack == 2 ? malloc((size_t)1 << 18) : NULL;
    if (ax_co == NULL || ax_main == NULL || ax_hc == NULL || (stack == 2 && heap == NULL) || getcontext(ax_co) != 0) return -1;
    ax_co->uc_stack.ss_sp = stack == 2 ? heap : (void *)local;
    ax_co->uc_stack.ss_size = stack == 2 ? (size_t)1 << 18 : sizeof local;
    ax_co->uc_link = ax_main;
    makecontext(ax_co, ax_body, 0);
    if (cr_on(SIGUSR1, ax_handler, onstack) != 0) return -2;
    swapcontext(ax_main, ax_co);
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    return cr_got;
}

#if defined(__aarch64__) || defined(__arm64__)
/* CU: A BY AN UNSEEN SWITCH, the Darwin seat's `au_*` (an arm64 switch in a
 * naked function, the coroutine on an array of the thread's own stack) with
 * the handler on the alternate stack where `onstack` (the engineer's
 * `au_*_1`, which Darwin did not write). MUST BE REFUSED, every form. */
__attribute__((naked)) static void cu_switch(void **from_sp, void *to_sp) {
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
static void *cu_h_sp, *cu_co_sp;
__attribute__((noreturn)) static void cu_entry(void) {
    if (cr_how == 1) cr_got = cr_down(12, 41);
    cu_switch(&cu_co_sp, cu_h_sp);
    __builtin_trap();
}
static void cu_handler(int sig) {
    (void)sig;
    cu_switch(&cu_h_sp, cu_co_sp);
    if (cr_how == 2) cr_got = cr_down(12, 42);
}
__attribute__((noinline)) static int64_t cu_run(int32_t how, int32_t onstack, cr_cb cb) {
    _Alignas(16) char stack[(size_t)1 << 16];
    cr_how = how;
    cr_f = cb;
    cr_got = -7;
    uintptr_t top = ((uintptr_t)stack + sizeof stack) & ~(uintptr_t)15;
    uint64_t *frame = (uint64_t *)(top - 176);
    memset(frame, 0, 176);
    frame[11] = (uint64_t)(uintptr_t)cu_entry;
    cu_co_sp = frame;
    if (cr_on(SIGUSR1, cu_handler, onstack) != 0) return -2;
    raise(SIGUSR1);
    __asm__ volatile("" ::"r"(stack) : "memory");
    return cr_got;
}
#endif
#endif

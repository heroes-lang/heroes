/* Defect 2745, lane-d2745: a jump made by a handler that runs NESTED inside
 * another (SIGUSR2 raised inside SIGUSR1's handler, both left unblocked to
 * each other), judged by the stack its target lies on, on route C and on
 * route A alike (`HERO_JMP_FORCE_UNCALIBRATED`, run apart, says the same).
 * Portable: Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`). The runner
 * returns what the callback returned, or a negative number where C itself
 * failed; both handlers run on an alternate stack where `onstack`.
 *
 * form 0: the inner handler jumps to a buffer the runner saved before either
 *   handler began: both are left, and the runner calls the program.
 *   MUST RUN.
 * form 1: the inner handler jumps to a buffer the outer handler saved; the
 *   outer handler, running again, calls the program. MUST BE REFUSED.
 * form 2: the inner handler jumps INTO a coroutine, through a buffer the
 *   coroutine saved before either signal (panel 255's A'' made from a nested
 *   handler); the coroutine resumes the inner handler's own context, which
 *   calls the program. The coroutine's stack: 0 an array in the runner's live
 *   frame, on the thread's own stack; 2 the heap. MUST BE REFUSED.
 * form 3: the inner handler jumps to a buffer the outer handler saved; the
 *   outer handler then returns, and the runner calls the program. MUST RUN. */
#ifndef FIXEDBUGS_2745_NESTED_H
#define FIXEDBUGS_2745_NESTED_H
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
#include <sys/mman.h>
#include <ucontext.h>

typedef int64_t (*nj_cb)(int64_t);
static nj_cb nj_f;
static volatile int64_t nj_got = -7;
static volatile int nj_form, nj_phase;
static sigjmp_buf nj_outer, nj_inner, nj_kb;
static ucontext_t *nj_co, *nj_main, *nj_hc;

__attribute__((noinline)) static int64_t nj_down(int n, int64_t v) {
    if (n == 0) return nj_f(v);
    int64_t r = nj_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}

static void nj_inner_handler(int sig) {
    (void)sig;
    if (nj_form == 0) siglongjmp(nj_outer, 1);
    if (nj_form != 2) siglongjmp(nj_inner, 1);
    nj_phase = 0;
    getcontext(nj_hc);
    if (nj_phase == 0) {
        nj_phase = 1;
        siglongjmp(nj_kb, 1);
    }
    nj_got = nj_down(12, 72);
}

static void nj_outer_handler(int sig) {
    (void)sig;
    if (nj_form == 1 || nj_form == 3) {
        if (sigsetjmp(nj_inner, 1) == 0) {
            raise(SIGUSR2);
            return;
        }
        if (nj_form == 1) nj_got = nj_down(12, 71);
        return;
    }
    raise(SIGUSR2);
}

static void nj_body(void) {
    if (sigsetjmp(nj_kb, 0) == 0) swapcontext(nj_co, nj_main);
    setcontext(nj_hc);
}

static int nj_on(int sig, void (*h)(int), int onstack) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    return sigaction(sig, &sa, NULL);
}

__attribute__((noinline)) static int64_t nj_run(int32_t form, int32_t onstack, int32_t stack, nj_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    nj_f = cb;
    nj_form = form;
    nj_got = -7;
    if (onstack) {
        size_t size = (size_t)256 << 10;
        void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (alt == MAP_FAILED) return -1;
        stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
        if (sigaltstack(&ss, NULL) != 0) return -1;
    }
    if (nj_on(SIGUSR1, nj_outer_handler, onstack) != 0 || nj_on(SIGUSR2, nj_inner_handler, onstack) != 0) return -2;
    if (form == 2) {
        nj_co = calloc(1, sizeof(ucontext_t));
        nj_main = calloc(1, sizeof(ucontext_t));
        nj_hc = calloc(1, sizeof(ucontext_t));
        void *heap = stack == 2 ? malloc((size_t)1 << 18) : NULL;
        if (nj_co == NULL || nj_main == NULL || nj_hc == NULL || (stack == 2 && heap == NULL) || getcontext(nj_co) != 0) {
            return -3;
        }
        nj_co->uc_stack.ss_sp = stack == 2 ? heap : (void *)local;
        nj_co->uc_stack.ss_size = stack == 2 ? (size_t)1 << 18 : sizeof local;
        nj_co->uc_link = nj_main;
        makecontext(nj_co, nj_body, 0);
        swapcontext(nj_main, nj_co);
    }
    if (sigsetjmp(nj_outer, 1) == 0) {
        raise(SIGUSR1);
        __asm__ volatile("" ::"r"(local) : "memory");
        if (form == 3) return nj_down(12, 73);
        return nj_got;
    }
    return nj_down(12, 70);
}
#endif

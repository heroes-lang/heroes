/* fixedbugs-2707-two-stacks.h: a CORRECT jump out of a signal handler that runs on an alternate stack, into a
 * coroutine that runs on a stack of its own (`makecontext`); the handler is left for good, then the coroutine
 * calls the program twelve frames down and returns to the runner through `uc_link`. Both stacks are `mmap`ed,
 * so which of the two lies higher in memory is the order the program mapped them in and no allocator's:
 *   order 0: the coroutine's stack is mapped first, the alternate stack second;
 *   order 1: the alternate stack is mapped first (set up at start-up, as a program that installs its handlers
 *            early does), the coroutine's second.
 * How 0: the coroutine saves with `sigsetjmp` and the handler leaves by `siglongjmp`; how 1: `getcontext` and
 * `setcontext`. (Panel 257's compiler-engineer's `cs2`, parameterised and silent about the addresses, which
 * differ by platform.) */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>
static int64_t (*cs2_cb)(int64_t);
static ucontext_t cs2_main, cs2_co, cs2_mid;
static sigjmp_buf cs2_env;
static volatile int cs2_phase, cs2_ctx;
static volatile int64_t cs2_got = -7;
__attribute__((noinline)) static int64_t cs2_down(int n, int64_t v) {
    if (n == 0) return cs2_cb(v);
    int64_t r = cs2_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static void cs2_handler(int sig) {
    (void)sig;
    if (cs2_ctx) setcontext(&cs2_mid);
    siglongjmp(cs2_env, 1);
}
static void cs2_body(void) {
    cs2_phase = 0;
    if (cs2_ctx) getcontext(&cs2_mid);
    if (cs2_ctx ? cs2_phase == 0 : sigsetjmp(cs2_env, 1) == 0) {
        cs2_phase = 1;
        raise(SIGUSR1);
        return;
    }
    cs2_got = cs2_down(12, 90);
}
static void *cs2_map(size_t size) { return mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0); }
static inline int64_t cs2_run(int64_t order, int64_t how, int64_t (*cb)(int64_t)) {
    cs2_ctx = how != 0;
    cs2_cb = cb;
    size_t size = (size_t)256 << 10;
    void *alt, *stack;
    if (order) { alt = cs2_map(size); stack = cs2_map(size); } else { stack = cs2_map(size); alt = cs2_map(size); }
    if (alt == MAP_FAILED || stack == MAP_FAILED) return -1;
    stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
    if (sigaltstack(&ss, NULL) != 0 || getcontext(&cs2_co) != 0) return -2;
    cs2_co.uc_stack.ss_sp = stack;
    cs2_co.uc_stack.ss_size = size;
    cs2_co.uc_link = &cs2_main;
    makecontext(&cs2_co, cs2_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = cs2_handler;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -3;
    swapcontext(&cs2_main, &cs2_co);
    return cs2_got;
}

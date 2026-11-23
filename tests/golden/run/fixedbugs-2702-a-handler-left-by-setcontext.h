/* Panel 255, round two, the ffi-pragmatist: c_* spelled with setcontext. A
 * coroutine on a heap stack takes its own context mid-run (getcontext) and
 * raises; the handler (on the coroutine's stack where `onstack` is 0, on the
 * alternate stack where 1) leaves by setcontext to that context, saving
 * nothing of its own, and the coroutine, the handler left for good, calls the
 * program twelve frames down, then returns to the runner through uc_link.
 * Correct by the reasoning that makes c_* MUST RUN. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
static int64_t (*cs_cb)(int64_t);
static ucontext_t *cs_main, *cs_co, *cs_mid;
static volatile int cs_phase;
static volatile int64_t cs_got = -7;
__attribute__((noinline)) static int64_t cs_down(int n, int64_t v) {
    if (n == 0) return cs_cb(v);
    int64_t r = cs_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static void cs_handler(int sig) {
    (void)sig;
    setcontext(cs_mid);
}
static void cs_body(void) {
    cs_phase = 0;
    getcontext(cs_mid);
    if (cs_phase == 0) {
        cs_phase = 1;
        raise(SIGUSR1);
        return;
    }
    cs_got = cs_down(12, 90);
}
static inline int64_t cs_run(int32_t onstack, int64_t (*cb)(int64_t)) {
    cs_cb = cb;
    cs_main = calloc(1, sizeof(ucontext_t));
    cs_co = calloc(1, sizeof(ucontext_t));
    cs_mid = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 18);
    if (cs_main == NULL || cs_co == NULL || cs_mid == NULL || stack == NULL || getcontext(cs_co) != 0) return -1;
    cs_co->uc_stack.ss_sp = stack;
    cs_co->uc_stack.ss_size = (size_t)1 << 18;
    cs_co->uc_link = cs_main;
    makecontext(cs_co, cs_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = cs_handler;
    sigemptyset(&sa.sa_mask);
    if (onstack) {
        size_t size = (size_t)256 << 10;
        void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
        if (alt == MAP_FAILED || sigaltstack(&ss, NULL) != 0) return -2;
        sa.sa_flags = SA_ONSTACK;
    }
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -3;
    swapcontext(cs_main, cs_co);
    return cs_got;
}

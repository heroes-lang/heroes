/* Panel 255, round two, the ffi-pragmatist: R, a handler left by a jump on
 * its own stack and then resumed by a jump back INTO its frames. The handler
 * saves (spell 0: sigsetjmp; spell 1: getcontext), jumps out to the runner's
 * buffer, and the runner at once jumps back to the handler's save; the
 * handler, running again, calls the program twelve frames down. The jump back
 * is undefined in C for spell 0 (C11 7.13.2.1: the function that called
 * setjmp has terminated); it works on this Mac in practice. Whatever its
 * standing, the call it makes is from inside a running handler: MUST BE
 * REFUSED. */
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
static int64_t (*rr_cb)(int64_t);
static sigjmp_buf rr_out, rr_in;
static ucontext_t *rr_hc;
static volatile int rr_spell, rr_back;
static volatile int64_t rr_got = -7;
__attribute__((noinline)) static int64_t rr_down(int n, int64_t v) {
    if (n == 0) return rr_cb(v);
    int64_t r = rr_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static ucontext_t rr_co;
static void rr_co_body(void) { setcontext(rr_hc); }
static void rr_handler(int sig) {
    (void)sig;
    rr_back = 0;
    if (rr_spell == 0) {
        if (sigsetjmp(rr_in, 0) == 0) siglongjmp(rr_out, 1);
    } else { /* spells 1 and 2 */
        getcontext(rr_hc);
        if (rr_back == 0) siglongjmp(rr_out, 1);
    }
    rr_got = rr_down(12, 80 + rr_spell);
}
/* The signal raised 16 KiB below the runner, so the jump back, made from
 * the runner's frame, writes nothing over the handler's frames. */
__attribute__((noinline)) static void rr_raise_deep(void) {
    volatile char pad[16384];
    pad[0] = 1;
    __asm__ volatile("" ::"r"(pad) : "memory"); /* kept whole at -O2 (step 199p) */
    raise(SIGUSR1);
    (void)pad[0];
}
__attribute__((noinline)) static int64_t rr_run(int32_t spell, int64_t (*cb)(int64_t)) {
    rr_cb = cb;
    rr_spell = spell;
    rr_hc = calloc(1, sizeof(ucontext_t));
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = rr_handler;
    sigemptyset(&sa.sa_mask);
    if (rr_hc == NULL || sigaction(SIGUSR1, &sa, NULL) != 0) return -1;
    if (sigsetjmp(rr_out, 1) == 0) {
        rr_raise_deep();
        return rr_got;
    }
    if (rr_got != -7) return rr_got;
    rr_back = 1;
    if (spell == 0) siglongjmp(rr_in, 1);
    if (spell == 2) {
        /* spell 2: the jump back made from a coroutine on a heap stack, so it
         * comes from another range than the handler's frames. */
        ucontext_t *main_uc = calloc(1, sizeof(ucontext_t));
        void *stack = malloc((size_t)1 << 18);
        if (main_uc == NULL || stack == NULL || getcontext(&rr_co) != 0) return -4;
        rr_co.uc_stack.ss_sp = stack;
        rr_co.uc_stack.ss_size = (size_t)1 << 18;
        rr_co.uc_link = main_uc;
        makecontext(&rr_co, rr_co_body, 0);
        swapcontext(main_uc, &rr_co);
        return rr_got;
    }
    setcontext(rr_hc);
    return -3;
}

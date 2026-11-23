/* Panel 255, round two, the ffi-pragmatist: rule 5's premise attacked. A
 * handler saves a buffer of its own, swaps to a coroutine on a heap stack,
 * and the coroutine resumes the handler by siglongjmp to that buffer (not
 * through the swap); the handler, running again, leaves for good by the
 * plainest jump out to the runner's buffer, and the runner calls the program
 * 64 KiB deeper. Correct: the handler was resumed and then left. MUST RUN. */
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
static int64_t (*sp_cb)(int64_t);
static sigjmp_buf sp_out, sp_hb;
static ucontext_t *sp_tmp, *sp_co;
static void sp_body(void) { siglongjmp(sp_hb, 1); }
static void sp_handler(int sig) {
    (void)sig;
    if (sigsetjmp(sp_hb, 0) == 0) swapcontext(sp_tmp, sp_co);
    siglongjmp(sp_out, 1);
}
__attribute__((noinline)) static int64_t sp_deep(int64_t v) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = sp_cb(v);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}
__attribute__((noinline)) static int64_t sp_run(int64_t (*cb)(int64_t)) {
    sp_cb = cb;
    sp_tmp = calloc(1, sizeof(ucontext_t));
    sp_co = calloc(1, sizeof(ucontext_t));
    _Alignas(16) char stack_array[(size_t)1 << 18]; /* an array of the runner's frame: AddressSanitizer warns of every jump made on a heap stack */
    void *stack = stack_array;
    if (sp_tmp == NULL || sp_co == NULL || stack == NULL || getcontext(sp_co) != 0) return -1;
    sp_co->uc_stack.ss_sp = stack;
    sp_co->uc_stack.ss_size = (size_t)1 << 18;
    sp_co->uc_link = NULL;
    makecontext(sp_co, sp_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sp_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    if (sigsetjmp(sp_out, 1) == 0) {
        raise(SIGUSR1);
        return -3;
    }
    return sp_deep(95);
}

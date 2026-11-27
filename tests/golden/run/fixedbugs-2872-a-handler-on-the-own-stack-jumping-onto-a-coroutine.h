/* ONSTACK's mirror of defect 2872 (lane sig-family-3): a signal handler on the
 * thread's own stack jumps onto a coroutine's stack.
 * mo_run(save, heap, cb): a coroutine, started by the runner's `swapcontext`
 *   on a heap stack (`heap` 1) or on an array of the runner's live frame
 *   (`heap` 0), saves a buffer and swaps back; the runner raises a signal; the
 *   handler, on the thread's own stack, jumps into the coroutine through the
 *   buffer (`siglongjmp`), and the coroutine calls the program.
 *   save 0: the handler saves nothing, so nothing can switch back into its
 *     frames; the coroutine then jumps home. The handler is left for good, as
 *     `ck`'s is: MUST RUN.
 *   save 1: the handler first saves its own point (`sigsetjmp`), which the
 *     coroutine jumps back through once it has called the program, and the
 *     handler then returns: the handler is parked while the program runs on
 *     the coroutine, MUST BE REFUSED. */
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

typedef int64_t (*mo_cb)(int64_t);
static ucontext_t *mo_main, *mo_co;
static sigjmp_buf mo_kb, mo_back, mo_hb;
static volatile int mo_save;
static mo_cb mo_f;
static volatile int64_t mo_got = -7;

/* The coroutine's own body is not instrumented by `--sanitize`: clang's
 * AddressSanitizer asks ASan to clear the stack's shadow before its
 * `siglongjmp`, and on a heap stack, which ASan was never told of, ASan writes
 * a warning instead (defect 2896, the program's own C); the runtime's rewrite
 * of the jump clears the coroutine's range itself (step 199l). It reached
 * that jump once the handler's jump into it ran (step 199r). */
__attribute__((no_sanitize("address"))) static void mo_body(void) {
    if (sigsetjmp(mo_kb, 0) == 0) {
        swapcontext(mo_co, mo_main);
        return;
    }
    mo_got = mo_f(6);
    if (mo_save) siglongjmp(mo_hb, 1);
    siglongjmp(mo_back, 1);
}

static void mo_handler(int sig) {
    (void)sig;
    if (mo_save) {
        if (sigsetjmp(mo_hb, 0) != 0) return;
    }
    siglongjmp(mo_kb, 1);
}

__attribute__((noinline)) static int64_t mo_run(int32_t save, int32_t heap, mo_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    mo_f = cb;
    mo_save = save;
    mo_main = calloc(1, sizeof(ucontext_t));
    mo_co = calloc(1, sizeof(ucontext_t));
    void *stack = heap ? malloc((size_t)1 << 18) : (void *)local;
    size_t size = heap ? (size_t)1 << 18 : sizeof local;
    if (mo_main == NULL || mo_co == NULL || stack == NULL || getcontext(mo_co) != 0) return -1;
    mo_co->uc_stack.ss_sp = stack;
    mo_co->uc_stack.ss_size = size;
    mo_co->uc_link = mo_main;
    makecontext(mo_co, mo_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = mo_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR2, &sa, NULL) != 0) return -2;
    swapcontext(mo_main, mo_co);
    if (sigsetjmp(mo_back, 1) == 0) raise(SIGUSR2);
    __asm__ volatile("" ::"r"(local) : "memory");
    if (heap) free(stack);
    free(mo_main);
    free(mo_co);
    return mo_got;
}

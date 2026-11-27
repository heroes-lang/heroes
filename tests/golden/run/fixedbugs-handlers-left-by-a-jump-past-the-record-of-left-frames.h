/* Defect d-lane-sig-family-4 (lane sig-family): panel 262's compiler-engineer's
 * `wide.h`, the count an argument. A coroutine (`makecontext` once, then
 * `_setjmp`/`_longjmp`, as `mj.h`) on an `mmap`'d stack runs the callback and
 * yields, saving where it stands. Then C leaves handlers by `siglongjmp`: one
 * on an alternate stack of its own (a static array), then `n` on the thread's
 * own stack, deepest first, each at its own depth, so no save point made later
 * lies below an earlier one's frames. Then C resumes the coroutine by
 * `_longjmp` to its save point, and the coroutine calls the callback again. No
 * handler is ever jumped back into: every one is left for good, and the
 * coroutine's save point was made before any of them ran. A correct program:
 * MUST RUN, printing 102, whatever `n`; 8 (nine spans) passes step 199d's
 * record of eight, 20 (twenty-one) this step's sixteen. */
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
typedef int64_t (*wi_cb_t)(int64_t);
#define WI_STACK (256u << 10)
static char wi_alt[1u << 16] __attribute__((aligned(16)));
static sigjmp_buf wi_env;
static ucontext_t wi_uc;
static jmp_buf wi_main_env, wi_co_env;
static wi_cb_t wi_fn;
static int64_t wi_out;
static void wi_leave(int sig) {
    (void)sig;
    siglongjmp(wi_env, 1);
}
/* The coroutine's own body is not instrumented by `--sanitize`: clang's
 * AddressSanitizer asks ASan to clear the stack's shadow before each
 * `_longjmp`, and on this `mmap`'d stack, which ASan was never told of, ASan
 * writes a warning instead (defect 2896, the program's own C); the
 * runtime's rewrite of the jump clears the coroutine's range itself (step
 * 199l). */
__attribute__((no_sanitize("address"))) static void wi_entry(void) {
    wi_out = wi_fn(1);
    if (!_setjmp(wi_co_env)) _longjmp(wi_main_env, 1);
    wi_out = wi_fn(2);
    _longjmp(wi_main_env, 1);
}
__attribute__((noinline)) static int wi_raise_at(int sig, int depth) {
    volatile char *pad = __builtin_alloca((size_t)depth * 16384 + 64);
    pad[0] = 1;
    if (sigsetjmp(wi_env, 1) == 0) {
        raise(sig);
        return -1;
    }
    return pad[0];
}
static inline int64_t wi_run(int32_t n, wi_cb_t cb) {
    char *stack = mmap(NULL, WI_STACK, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (stack == MAP_FAILED) return -2;
    wi_fn = cb;
    getcontext(&wi_uc);
    wi_uc.uc_stack.ss_sp = stack;
    wi_uc.uc_stack.ss_size = WI_STACK;
    wi_uc.uc_link = NULL;
    makecontext(&wi_uc, wi_entry, 0);
    if (!_setjmp(wi_main_env)) setcontext(&wi_uc);
    int64_t first = wi_out;
    stack_t ss;
    memset(&ss, 0, sizeof ss);
    ss.ss_sp = wi_alt;
    ss.ss_size = sizeof wi_alt;
    if (sigaltstack(&ss, NULL) != 0) return -3;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = wi_leave;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -4;
    sa.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa, NULL) != 0) return -4;
    if (wi_raise_at(SIGUSR1, 0) != 1) return -5;
    for (int k = n; k >= 1; k--)
        if (wi_raise_at(SIGUSR2, k) != 1) return -5;
    if (!_setjmp(wi_main_env)) _longjmp(wi_co_env, 1);
    return first * 100 + wi_out;
}

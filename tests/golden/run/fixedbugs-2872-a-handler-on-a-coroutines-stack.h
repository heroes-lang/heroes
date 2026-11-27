/* Defect 2872 (lane sig-family, step 199m): panel 262's completeness critic's
 * `ck.h`, its three switches parameters here and a fourth added: `save`, a
 * save point the handler makes inside its own frames before it leaves, which
 * the runner jumps back into after the callback: there the callback is made
 * while the handler is still to run again, and MUST BE REFUSED; without it
 * every cell MUST RUN. The critic's words follow.
 *
 * ck.h (panel 262, the completeness critic's second reading): w2's exit, "back to a call the signal interrupted",
 * from a handler that interrupted a coroutine. A coroutine makecontext made on a heap stack raises SIGUSR1; the
 * handler runs on the coroutine's stack (CK_ON 0) or on an alternate stack (CK_ON 1) and leaves by siglongjmp
 * (CK_HOW 0) or by setcontext to a context getcontext took (CK_HOW 1), either back into the coroutine, to a save it
 * made just before it raised (CK_TO 1), or to the runner, to a save made before it switched into the coroutine
 * (CK_TO 0, the coroutine then left suspended for good). Then the program's function is called twelve frames down,
 * by the coroutine (CK_TO 1, which returns to the runner through uc_link) or by the runner (CK_TO 0). The signal is
 * raise()'s, so every jump here is one POSIX defines. */
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
typedef int64_t (*ck_cb_t)(int64_t);
static ck_cb_t ck_cb;
static ucontext_t ck_main, ck_co, ck_ctx;
static sigjmp_buf ck_env;
static volatile int ck_how, ck_to, ck_phase, ck_save;
static sigjmp_buf ck_inside;
static volatile int64_t ck_got = -7;
__attribute__((noinline)) static int64_t ck_down(int n, int64_t v) {
    if (n == 0) return ck_cb(v);
    int64_t r = ck_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
/* The handler is not instrumented by `--sanitize`: clang's AddressSanitizer
 * asks ASan to clear the stack's shadow before its `siglongjmp`, and on the
 * coroutine's heap stack, which ASan was never told of, ASan writes a warning
 * instead (defect 2896, the program's own C); the runtime's rewrite of the
 * jump clears the coroutine's range itself (step 199l). */
__attribute__((no_sanitize("address"))) static void ck_handler(int sig) {
    (void)sig;
    if (ck_save && sigsetjmp(ck_inside, 0) != 0) return;
    if (ck_how == 0) siglongjmp(ck_env, 1);
    setcontext(&ck_ctx);
}
static void ck_body(void) {
    if (ck_to == 1) {
        ck_phase = 0;
        if (ck_how == 0) {
            if (sigsetjmp(ck_env, 1) != 0) {
                ck_got = ck_down(12, 7);
                return;
            }
        } else {
            getcontext(&ck_ctx);
            if (ck_phase != 0) {
                ck_got = ck_down(12, 7);
                return;
            }
        }
        ck_phase = 1;
    }
    raise(SIGUSR1);
}
static inline int64_t ck_run(int32_t onstack, int32_t how, int32_t to, int32_t save, ck_cb_t cb) {
    ck_how = how;
    ck_to = to;
    ck_save = save;
    ck_cb = cb;
    size_t size = (size_t)256 << 10;
    void *stack = malloc(size);
    if (stack == NULL || getcontext(&ck_co) != 0) return -1;
    ck_co.uc_stack.ss_sp = stack;
    ck_co.uc_stack.ss_size = size;
    ck_co.uc_link = &ck_main;
    makecontext(&ck_co, ck_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = ck_handler;
    sigemptyset(&sa.sa_mask);
    if (onstack) {
        void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
        if (alt == MAP_FAILED || sigaltstack(&ss, NULL) != 0) return -2;
        sa.sa_flags = SA_ONSTACK;
    }
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -3;
    if (ck_to == 1) {
        swapcontext(&ck_main, &ck_co);
        return ck_got;
    }
    ck_phase = 0;
    if (ck_how == 0) {
        if (sigsetjmp(ck_env, 1) != 0) {
            int64_t r = ck_down(12, 7);
            if (ck_save) siglongjmp(ck_inside, 1);
            return r;
        }
    } else {
        getcontext(&ck_ctx);
        if (ck_phase != 0) {
            int64_t r = ck_down(12, 7);
            if (ck_save) siglongjmp(ck_inside, 1);
            return r;
        }
    }
    ck_phase = 1;
    swapcontext(&ck_main, &ck_co);
    return -4;
}

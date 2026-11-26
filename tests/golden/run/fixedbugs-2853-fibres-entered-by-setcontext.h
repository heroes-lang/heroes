/* Defect 2853 (lane sig-family): OpenSSL ASYNC's switch written out with no
 * OpenSSL (panel 259's critic's `mj.h`, its `async_posix.c`'s design as
 * libcrypto 3.5.9 imports it on Darwin: getcontext, makecontext, setcontext,
 * _setjmp, _longjmp): each fibre is made by `makecontext` on a quarter of an
 * array in `mj_host`'s frame, entered the first time by `setcontext` and every
 * time after by `_longjmp`, and every switch saves where it was by `_setjmp`.
 * The fibre's saved frame pointer is cleared after `makecontext` on Darwin
 * arm64, so the frame walk of defect 2852 is out of this case; the stack the
 * fibres are carved from is zeroed first. A correct program: MUST RUN, under
 * `--sanitize` too, where the frame of the runtime's own `setcontext`, left
 * by the switch, must leave nothing in AddressSanitizer's shadow. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <stdint.h>
#include <string.h>
#include <ucontext.h>
typedef int64_t (*mj_cb_t)(int64_t);
#define MJ_N 2
#define MJ_STACK (256u << 10)
typedef struct {
    ucontext_t uc;
    jmp_buf env;
    int env_init;
} mj_fibre;
static mj_fibre mj_main, mj_fib[MJ_N];
static mj_cb_t mj_fn[MJ_N];
static int64_t mj_out[MJ_N];
static int mj_now;
static char *mj_carved;
/* OpenSSL's async_fibre_swapcontext: save where o is, then enter n by its jump or, the first time, its context. */
static void mj_swap(mj_fibre *o, mj_fibre *n) {
    o->env_init = 1;
    if (!_setjmp(o->env)) {
        if (n->env_init) _longjmp(n->env, 1);
        else setcontext(&n->uc);
    }
}
static void mj_entry(void) {
    int k = mj_now;
    mj_out[k] = mj_fn[k](k);
    for (;;) mj_swap(&mj_fib[k], &mj_main);
}
static inline void mj_start(int64_t k, mj_cb_t f) {
    mj_fn[k] = f;
    getcontext(&mj_fib[k].uc);
    mj_fib[k].uc.uc_stack.ss_sp = mj_carved + k * MJ_STACK;
    mj_fib[k].uc.uc_stack.ss_size = MJ_STACK;
    mj_fib[k].uc.uc_link = NULL;
    makecontext(&mj_fib[k].uc, mj_entry, 0);
#if defined(__APPLE__) && defined(__aarch64__)
    mj_fib[k].uc.uc_mcontext->__ss.__fp = 0;
#endif
    mj_fib[k].env_init = 0;
    mj_now = (int)k;
    mj_swap(&mj_main, &mj_fib[k]);
}
static inline void mj_yield(void) { mj_swap(&mj_fib[mj_now], &mj_main); }
static inline int64_t mj_resume(int64_t k) {
    mj_now = (int)k;
    mj_swap(&mj_main, &mj_fib[k]);
    return mj_out[k];
}
static inline int64_t mj_host(mj_cb_t body, int64_t n) {
    char stacks[MJ_N * MJ_STACK];
    memset(stacks, 0, sizeof stacks);
    mj_carved = stacks;
    int64_t r = body(n);
    mj_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return r;
}

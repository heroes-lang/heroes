/* Defect d-lane-sig-family-3 (lane sig-family): `mj.h` (defect 2852's
 * `mj_carved`) with no `makecontext` (panel 262's compiler-engineer's
 * `fm_common.h` with `fm_top.h`): each fibre is started on a quarter of an
 * array in `fm_host`'s frame by an assembly jump of this header's own, its
 * frame pointer at its stack's top, as Darwin's `makecontext` leaves it, and
 * every later switch is `_setjmp`/`_longjmp` as in OpenSSL's `async_posix.c`.
 * No signal handler is installed anywhere: a correct program, MUST RUN,
 * under `--sanitize` too. AArch64 and x86-64 System V. */
#define FM_X29(top) (top)
#if defined(_WIN32)
#error "this fibre's start is System V's; Windows keeps other registers across a call"
#endif
#if !defined(__aarch64__) && !defined(__x86_64__)
#error "AArch64 and x86-64 only"
#endif
#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>
typedef int64_t (*fm_cb_t)(int64_t);
#define FM_N 2
#define FM_STACK (256u << 10)
typedef struct {
    jmp_buf env;
    int env_init;
} fm_fibre;
static fm_fibre fm_main, fm_fib[FM_N];
static fm_cb_t fm_fn[FM_N];
static int64_t fm_out[FM_N];
static int fm_now;
static char *fm_carved;
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
void __asan_handle_no_return(void);
#define FM_UNPOISON() __asan_handle_no_return()
#endif
#endif
#if !defined(FM_UNPOISON)
#define FM_UNPOISON() ((void)0)
#endif
/* sp and x29 set, no return address, a branch to the entry: what makecontext and setcontext do between them. */
#if defined(__aarch64__)
__attribute__((naked, noinline)) static void fm_boot(uintptr_t top, void (*entry)(void), uintptr_t fp) {
    __asm__ volatile("mov sp, x0\n\t"
                     "mov x29, x2\n\t"
                     "mov x30, xzr\n\t"
                     "br x1\n\t");
}
#else
/* x86-64: rsp at the top, a zero return address pushed below it, rbp the
 * frame pointer, a jump to the entry. */
__attribute__((naked, noinline)) static void fm_boot(uintptr_t top, void (*entry)(void), uintptr_t fp) {
    __asm__ volatile("mov %rdi, %rsp\n\t"
                     "mov %rdx, %rbp\n\t"
                     "pushq $0\n\t"
                     "jmp *%rsi\n\t");
}
#endif
static void fm_entry(void);
static void fm_swap(fm_fibre *o, fm_fibre *n, int64_t k) {
    o->env_init = 1;
    if (!_setjmp(o->env)) {
        if (n->env_init) _longjmp(n->env, 1);
        uintptr_t top = (uintptr_t)(fm_carved + (k + 1) * FM_STACK) & ~(uintptr_t)15;
        FM_UNPOISON();
        fm_boot(top, fm_entry, FM_X29(top));
    }
}
static void fm_entry(void) {
    int k = fm_now;
    fm_out[k] = fm_fn[k](k);
    for (;;) fm_swap(&fm_fib[k], &fm_main, k);
}
static inline void fm_start(int64_t k, fm_cb_t f) {
    fm_fn[k] = f;
    fm_fib[k].env_init = 0;
    fm_now = (int)k;
    fm_swap(&fm_main, &fm_fib[k], k);
}
static inline void fm_yield(void) { fm_swap(&fm_fib[fm_now], &fm_main, fm_now); }
static inline int64_t fm_resume(int64_t k) {
    fm_now = (int)k;
    fm_swap(&fm_main, &fm_fib[k], k);
    return fm_out[k];
}
static inline int64_t fm_host(fm_cb_t body, int64_t n) {
    char stacks[FM_N * FM_STACK];
    fm_carved = stacks;
    int64_t r = body(n);
    fm_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return r;
}

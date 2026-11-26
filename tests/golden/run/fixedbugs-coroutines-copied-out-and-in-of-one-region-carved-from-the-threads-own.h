/* fixedbugs-coroutines-copied-out-and-in-of-one-region-carved-from-the-threads-own.h (panel 259's
 * compiler-engineer, `copy.h`): two coroutines that run in turn on ONE region of the thread's own stack (an array
 * in `cp_host`'s frame), as a stack-copying library runs them: switching a coroutine in copies its saved image back
 * to the region, at the addresses it had, after copying the coroutine there out to the heap. So both coroutines'
 * frames hold the same addresses in turn. Switched by the assembly of the header it includes, always through the
 * host, whose frames lie under the region. A jump inside a coroutine is the C library's `setjmp` and `longjmp`
 * (`cj_try`). */
#include <setjmp.h>
#include <string.h>
#include "fixedbugs-coroutines-switched-by-a-librarys-own-switch-on-stacks-carved-from-the-threads-own.h"
#define CP_N 2
#define CP_STACK (256u << 10)
static char *cp_top;
static void *cp_sp[CP_N], *cp_main;
static char *cp_save[CP_N];
static size_t cp_used[CP_N];
static ow_cb_t cp_fn[CP_N];
static int64_t cp_out[CP_N];
static int cp_now = -1;
static void cp_entry(int64_t k) {
    cp_out[k] = cp_fn[k](k);
    for (;;) ow_swap(&cp_sp[k], cp_main);
}
/* The coroutine in the region copied out to the heap. */
static void cp_out_now(void) {
    if (cp_now < 0) return;
    size_t u = (size_t)(cp_top - (char *)cp_sp[cp_now]);
    free(cp_save[cp_now]);
    cp_save[cp_now] = (char *)malloc(u);
    memcpy(cp_save[cp_now], cp_sp[cp_now], u);
    cp_used[cp_now] = u;
    cp_now = -1;
}
static inline void cp_start(int64_t k, ow_cb_t f) {
    cp_out_now();
    cp_fn[k] = f;
    uint64_t *fr = (uint64_t *)(void *)(cp_top - OW_FRAME);
    for (int i = 0; i < OW_FRAME / 8; i++) fr[i] = 0;
    ow_prime(fr, k, cp_entry);
    cp_sp[k] = fr;
    cp_now = (int)k;
    ow_swap(&cp_main, cp_sp[k]);
}
static inline void cp_yield(void) { ow_swap(&cp_sp[cp_now], cp_main); }
static inline int64_t cp_resume(int64_t k) {
    if (cp_now != (int)k) {
        cp_out_now();
        memcpy(cp_sp[k], cp_save[k], cp_used[k]);
        cp_now = (int)k;
    }
    ow_swap(&cp_main, cp_sp[k]);
    return cp_out[k];
}
static inline int64_t cp_host(ow_cb_t body, int64_t n) {
    char region[CP_STACK];
    cp_top = (char *)(((uintptr_t)(region + CP_STACK)) & ~(uintptr_t)15);
    int64_t r = body(n);
    __asm__ volatile("" ::"r"(region) : "memory");
    return r;
}
static jmp_buf cj_env[CP_N];
static inline int64_t cj_try(ow_cb_t cb, int64_t k) {
    if (setjmp(cj_env[k]) == 0) return cb(k);
    return 7;
}
static inline void cj_throw(int64_t k) { longjmp(cj_env[k], 1); }

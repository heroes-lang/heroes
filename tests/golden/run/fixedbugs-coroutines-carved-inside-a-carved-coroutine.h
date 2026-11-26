/* fixedbugs-coroutines-carved-inside-a-carved-coroutine.h (panel 259's completeness critic, `nest.h`): coroutines
 * nested in a coroutine. The outer ones are carved from `ow_host`'s array by the header it includes; inside one of
 * them, `ns_inner` carves two more from an array in ITS OWN frame, on the outer coroutine's stack, switched by the
 * same assembly, started, resumed 1 then 0; it answers a * 1000 + b. */
#include "fixedbugs-coroutines-switched-by-a-librarys-own-switch-on-stacks-carved-from-the-threads-own.h"
#define NS_N 2
#define NS_STACK (64u << 10)
static void *ns_main_sp, *ns_sp[NS_N];
static ow_cb_t ns_fn[NS_N];
static int64_t ns_arg[NS_N], ns_out[NS_N];
static int ns_now;
static void ns_entry(int64_t k) {
    ns_out[k] = ns_fn[k](ns_arg[k]);
    for (;;) ow_swap(&ns_sp[k], ns_main_sp);
}
static inline void ns_yield(void) { ow_swap(&ns_sp[ns_now], ns_main_sp); }
static inline int64_t ns_inner(ow_cb_t f, int64_t base) {
    char stacks[NS_N * NS_STACK];
    for (int k = 0; k < NS_N; k += 1) {
        ns_fn[k] = f;
        ns_arg[k] = base + k;
        char *top = (char *)(((uintptr_t)(stacks + (k + 1) * NS_STACK)) & ~(uintptr_t)15);
        uint64_t *fr = (uint64_t *)(void *)(top - OW_FRAME);
        for (int i = 0; i < OW_FRAME / 8; i++) fr[i] = 0;
        ow_prime(fr, k, ns_entry);
        ns_sp[k] = fr;
        ns_now = k;
        ow_swap(&ns_main_sp, ns_sp[k]);
    }
    ns_now = 1;
    ow_swap(&ns_main_sp, ns_sp[1]);
    int64_t b = ns_out[1];
    ns_now = 0;
    ow_swap(&ns_main_sp, ns_sp[0]);
    int64_t a = ns_out[0];
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return a * 1000 + b;
}

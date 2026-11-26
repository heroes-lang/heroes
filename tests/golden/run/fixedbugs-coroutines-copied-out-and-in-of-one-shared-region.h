/* fixedbugs-coroutines-copied-out-and-in-of-one-shared-region.h (panel 259's ffi-pragmatist, `cpo.h`, its route-D
 * tell taken out): a STACK-COPYING coroutine library, switched by the assembly of the header it includes (no
 * `makecontext`, no `swapcontext`): every coroutine runs on ONE shared region, in turn; before another runs there,
 * the one whose frames the region holds is copied out to the heap, and a coroutine resumed is copied back in first,
 * so two coroutines' frames hold THE SAME ADDRESSES in turn (the design of a shared-stack library: libaco's,
 * greenlet's over the thread's own stack, Tencent libco's on the heap). The region is the heap's (`cpo_start`
 * outside a host), or carved from a C helper's frame on the thread's own stack (`cpo_host`). */
#include "fixedbugs-coroutines-switched-by-a-librarys-own-switch-on-stacks-carved-from-the-threads-own.h"
#define CPO_N 2
#define CPO_REGION (512u << 10)
static char *cpo_region;
static void *cpo_sp[CPO_N], *cpo_main_sp;
static unsigned char *cpo_save[CPO_N];
static int cpo_on = -1; /* the coroutine whose frames the region holds now */
static int cpo_now;
static ow_cb_t cpo_fn[CPO_N];
static int64_t cpo_out[CPO_N];
static char *cpo_top(void) { return (char *)(((uintptr_t)(cpo_region + CPO_REGION)) & ~(uintptr_t)15); }
/* The whole region copied, by bytes no sanitizer watches (a coroutine's frames carry its redzones). */
__attribute__((no_sanitize("address"), noinline)) static void cpo_copy(volatile unsigned char *to, volatile const unsigned char *from, size_t n) {
    for (size_t i = 0; i < n; i += 1) to[i] = from[i];
}
static void cpo_park(void) {
    if (cpo_on < 0) return;
    if (cpo_save[cpo_on] == NULL) cpo_save[cpo_on] = malloc(CPO_REGION);
    cpo_copy(cpo_save[cpo_on], (unsigned char *)cpo_region, CPO_REGION);
    cpo_on = -1;
}
static void cpo_entry(int64_t k) {
    cpo_out[k] = cpo_fn[k](k);
    for (;;) ow_swap(&cpo_sp[k], cpo_main_sp);
}
static void cpo_ready(void) {
    if (cpo_region == NULL) cpo_region = malloc(CPO_REGION);
}
/* Starts coroutine k at the region's top, the region's last occupant copied out first. */
static inline void cpo_start(int64_t k, ow_cb_t f) {
    cpo_ready();
    cpo_park();
    cpo_fn[k] = f;
    uint64_t *fr = (uint64_t *)(void *)(cpo_top() - OW_FRAME);
    for (int i = 0; i < OW_FRAME / 8; i++) fr[i] = 0;
    ow_prime(fr, k, cpo_entry);
    cpo_sp[k] = fr;
    cpo_on = (int)k;
    cpo_now = (int)k;
    ow_swap(&cpo_main_sp, cpo_sp[k]);
}
static inline void cpo_yield(void) { ow_swap(&cpo_sp[cpo_now], cpo_main_sp); }
static inline int64_t cpo_resume(int64_t k) {
    if (cpo_on != (int)k) {
        cpo_park();
        cpo_copy((unsigned char *)cpo_region, cpo_save[k], CPO_REGION);
        cpo_on = (int)k;
    }
    cpo_now = (int)k;
    ow_swap(&cpo_main_sp, cpo_sp[k]);
    return cpo_out[k];
}
/* Runs body with the shared region carved from this frame, on the thread's own stack. */
static inline int64_t cpo_host(ow_cb_t body, int64_t n) {
    char region[CPO_REGION + 64];
    cpo_region = region;
    int64_t r = body(n);
    cpo_region = NULL;
    __asm__ volatile("" ::"r"(region) : "memory");
    return r;
}

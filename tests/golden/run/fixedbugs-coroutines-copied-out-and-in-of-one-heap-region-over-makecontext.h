/* fixedbugs-coroutines-copied-out-and-in-of-one-heap-region-over-makecontext.h (panel 259's ffi-pragmatist,
 * `cpu.h`): a stack-copying design over the C library's `makecontext` and `swapcontext`, as the program links them
 * (every switch one the runtime wraps): ONE shared region from the heap, every coroutine made onto it by
 * `makecontext`, the occupant copied out before another runs there and copied back before it is resumed. Two
 * coroutines' frames hold the same addresses in turn, and the runtime is told of every context made and sees every
 * switch. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <stdint.h>
#include <stdlib.h>
#include <ucontext.h>
/* UNDER AddressSanitizer the region is unpoisoned before a coroutine starts or is copied back in: its shadow holds
 * the redzones of the frames the region held last, another coroutine's at the same addresses, which ASan read as a
 * `stack-buffer-underflow` of a correct program (panel 259's ffi-pragmatist, and lane land259 under route E). */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#define CPU_UNPOISON(p, n) __asan_unpoison_memory_region((p), (n))
#endif
#endif
#if !defined(CPU_UNPOISON)
#define CPU_UNPOISON(p, n) ((void)0)
#endif
typedef int64_t (*cpu_cb_t)(int64_t);
#define CPU_N 2
#define CPU_REGION (512u << 10)
static char *cpu_region;
static ucontext_t cpu_main, cpu_ctx[CPU_N];
static unsigned char *cpu_save[CPU_N];
static int cpu_on = -1;
static int cpu_now;
static cpu_cb_t cpu_fn[CPU_N];
static int64_t cpu_out[CPU_N];
__attribute__((no_sanitize("address"), noinline)) static void cpu_copy(volatile unsigned char *to, volatile const unsigned char *from, size_t n) {
    for (size_t i = 0; i < n; i += 1) to[i] = from[i];
}
static void cpu_park(void) {
    if (cpu_on < 0) return;
    if (cpu_save[cpu_on] == NULL) cpu_save[cpu_on] = malloc(CPU_REGION);
    cpu_copy(cpu_save[cpu_on], (unsigned char *)cpu_region, CPU_REGION);
    cpu_on = -1;
}
static void cpu_entry(int k) {
    cpu_out[k] = cpu_fn[k](k);
    swapcontext(&cpu_ctx[k], &cpu_main);
}
static inline void cpu_start(int64_t k, cpu_cb_t f) {
    if (cpu_region == NULL) cpu_region = malloc(CPU_REGION);
    cpu_park();
    cpu_fn[k] = f;
    CPU_UNPOISON(cpu_region, CPU_REGION);
    getcontext(&cpu_ctx[k]);
    cpu_ctx[k].uc_stack.ss_sp = cpu_region;
    cpu_ctx[k].uc_stack.ss_size = CPU_REGION;
    cpu_ctx[k].uc_link = &cpu_main;
    makecontext(&cpu_ctx[k], (void (*)(void))cpu_entry, 1, (int)k);
    cpu_on = (int)k;
    cpu_now = (int)k;
    swapcontext(&cpu_main, &cpu_ctx[k]);
}
static inline void cpu_yield(void) { swapcontext(&cpu_ctx[cpu_now], &cpu_main); }
static inline int64_t cpu_resume(int64_t k) {
    if (cpu_on != (int)k) {
        cpu_park();
        CPU_UNPOISON(cpu_region, CPU_REGION);
        cpu_copy((unsigned char *)cpu_region, cpu_save[k], CPU_REGION);
        cpu_on = (int)k;
    }
    cpu_now = (int)k;
    swapcontext(&cpu_main, &cpu_ctx[k]);
    return cpu_out[k];
}

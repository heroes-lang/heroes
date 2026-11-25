/* fixedbugs-coroutines-switched-by-a-librarys-own-switch.h: up to four coroutines on the heap's stacks, switched by
 * a switch of this header's own, written in assembly as a coroutine library's is: no `makecontext`, no `swapcontext`,
 * nothing the runtime wraps, so the runtime cannot name the stacks. AArch64 and x86-64 System V, run on both (the
 * Linux x86-64 image and this Mac, 2026-11-25). Windows x64 keeps other registers across a call (`rdi`, `rsi` and
 * `xmm6` to `xmm15` besides), so this switch is refused there rather than built wrong, and the case is named in
 * `tests/harness/platform_skips.hero`'s `unbuildable_on_windows`. */
#if defined(_WIN32)
#error "this coroutine switch is System V's; Windows keeps other registers across a call"
#endif
#include <stdint.h>
#include <stdlib.h>
typedef int64_t (*ow_cb_t)(int64_t);
#define OW_N 4
#define OW_STACK (256u << 10)
static void *ow_main_sp, *ow_sp[OW_N];
static ow_cb_t ow_fn[OW_N];
static int64_t ow_out[OW_N];
static int ow_now;
static void *ow_heap[OW_N];
#if defined(__aarch64__)
__attribute__((naked, noinline)) static void ow_swap(void **from, void *to) {
    __asm__ volatile("sub sp, sp, #176\n stp x19, x20, [sp, #0]\n stp x21, x22, [sp, #16]\n stp x23, x24, [sp, #32]\n"
                     "stp x25, x26, [sp, #48]\n stp x27, x28, [sp, #64]\n stp x29, x30, [sp, #80]\n"
                     "stp d8, d9, [sp, #96]\n stp d10, d11, [sp, #112]\n stp d12, d13, [sp, #128]\n stp d14, d15, [sp, #144]\n"
                     "mov x9, sp\n str x9, [x0]\n mov sp, x1\n"
                     "ldp x19, x20, [sp, #0]\n ldp x21, x22, [sp, #16]\n ldp x23, x24, [sp, #32]\n"
                     "ldp x25, x26, [sp, #48]\n ldp x27, x28, [sp, #64]\n ldp x29, x30, [sp, #80]\n"
                     "ldp d8, d9, [sp, #96]\n ldp d10, d11, [sp, #112]\n ldp d12, d13, [sp, #128]\n ldp d14, d15, [sp, #144]\n"
                     "add sp, sp, #176\n ret\n");
}
__attribute__((naked, noinline)) static void ow_boot(void) { __asm__ volatile("mov x0, x19\n blr x20\n brk #1\n"); }
#define OW_FRAME 176
static void ow_prime(uint64_t *f, int64_t k, void (*entry)(int64_t)) {
    f[0] = (uint64_t)k;                    /* x19 */
    f[1] = (uint64_t)(uintptr_t)entry;     /* x20 */
    f[10] = 0;                             /* x29 */
    f[11] = (uint64_t)(uintptr_t)ow_boot;  /* x30 */
}
#elif defined(__x86_64__)
__attribute__((naked, noinline)) static void ow_swap(void **from, void *to) {
    __asm__ volatile("push %rbp\n push %rbx\n push %r12\n push %r13\n push %r14\n push %r15\n"
                     "mov %rsp, (%rdi)\n mov %rsi, %rsp\n"
                     "pop %r15\n pop %r14\n pop %r13\n pop %r12\n pop %rbx\n pop %rbp\n ret\n");
}
__attribute__((naked, noinline)) static void ow_boot(void) { __asm__ volatile("mov %rbx, %rdi\n call *%r12\n ud2\n"); }
#define OW_FRAME 56
static void ow_prime(uint64_t *f, int64_t k, void (*entry)(int64_t)) {
    f[3] = (uint64_t)(uintptr_t)entry;     /* r12 */
    f[4] = (uint64_t)k;                    /* rbx */
    f[5] = 0;                              /* rbp */
    f[6] = (uint64_t)(uintptr_t)ow_boot;   /* return address */
}
#else
#error "own.h: AArch64 and x86-64 only"
#endif
static void ow_entry(int64_t k) {
    ow_out[k] = ow_fn[k](k);
    for (;;) ow_swap(&ow_sp[k], ow_main_sp);
}
/* Starts coroutine k running f(k) until it yields; its stack is reused when k is started again. */
static inline void ow_start(int64_t k, ow_cb_t f) {
    ow_fn[k] = f;
    if (ow_heap[k] == NULL) ow_heap[k] = malloc(OW_STACK);
    char *base = (char *)ow_heap[k];
    char *top = (char *)(((uintptr_t)(base + OW_STACK)) & ~(uintptr_t)15);
    uint64_t *fr = (uint64_t *)(void *)(top - OW_FRAME);
    for (int i = 0; i < OW_FRAME / 8; i++) fr[i] = 0;
    ow_prime(fr, k, ow_entry);
    ow_sp[k] = fr;
    ow_now = (int)k;
    ow_swap(&ow_main_sp, ow_sp[k]);
}
static inline void ow_yield(void) { ow_swap(&ow_sp[ow_now], ow_main_sp); }
static inline int64_t ow_resume(int64_t k) {
    ow_now = (int)k;
    ow_swap(&ow_main_sp, ow_sp[k]);
    return ow_out[k];
}

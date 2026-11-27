/* unwind-sw.h (panel 262's blind programs' sw.h, for step 214c of panel 258's R2): up to four coroutines, each running a function of the program on a stack of its own, switched by this
 * header's own switch, written in assembly (AArch64 and x86-64 System V): it saves the callee-saved registers and
 * the stack pointer of the side that leaves and loads those of the side that resumes. A coroutine's stack is
 * memory from malloc, or, while sw_host runs, a quarter of an array in sw_host's own frame. */
#if defined(_WIN32)
#error "this coroutine switch is System V's; Windows keeps other registers across a call"
#endif
#include <stdint.h>
#include <stdlib.h>
typedef int64_t (*sw_fn_t)(int64_t);
#define SW_N 4
#define SW_STACK (256u << 10)
static void *sw_main_sp, *sw_sp[SW_N];
static sw_fn_t sw_fn[SW_N];
static int64_t sw_out[SW_N];
static int sw_now;
static char *sw_carved;
static void *sw_heap[SW_N];
#if defined(__aarch64__)
__attribute__((naked, noinline)) static void sw_swap(void **from, void *to) {
    __asm__ volatile("sub sp, sp, #176\n stp x19, x20, [sp, #0]\n stp x21, x22, [sp, #16]\n stp x23, x24, [sp, #32]\n"
                     "stp x25, x26, [sp, #48]\n stp x27, x28, [sp, #64]\n stp x29, x30, [sp, #80]\n"
                     "stp d8, d9, [sp, #96]\n stp d10, d11, [sp, #112]\n stp d12, d13, [sp, #128]\n stp d14, d15, [sp, #144]\n"
                     "mov x9, sp\n str x9, [x0]\n mov sp, x1\n"
                     "ldp x19, x20, [sp, #0]\n ldp x21, x22, [sp, #16]\n ldp x23, x24, [sp, #32]\n"
                     "ldp x25, x26, [sp, #48]\n ldp x27, x28, [sp, #64]\n ldp x29, x30, [sp, #80]\n"
                     "ldp d8, d9, [sp, #96]\n ldp d10, d11, [sp, #112]\n ldp d12, d13, [sp, #128]\n ldp d14, d15, [sp, #144]\n"
                     "add sp, sp, #176\n ret\n");
}
__attribute__((naked, noinline)) static void sw_boot(void) { __asm__ volatile("mov x0, x19\n blr x20\n brk #1\n"); }
#define SW_FRAME 176
static void sw_prime(uint64_t *f, int64_t k, void (*entry)(int64_t)) {
    f[0] = (uint64_t)k;
    f[1] = (uint64_t)(uintptr_t)entry;
    f[10] = 0;
    f[11] = (uint64_t)(uintptr_t)sw_boot;
}
#elif defined(__x86_64__)
__attribute__((naked, noinline)) static void sw_swap(void **from, void *to) {
    __asm__ volatile("push %rbp\n push %rbx\n push %r12\n push %r13\n push %r14\n push %r15\n"
                     "mov %rsp, (%rdi)\n mov %rsi, %rsp\n"
                     "pop %r15\n pop %r14\n pop %r13\n pop %r12\n pop %rbx\n pop %rbp\n ret\n");
}
__attribute__((naked, noinline)) static void sw_boot(void) { __asm__ volatile("mov %rbx, %rdi\n call *%r12\n ud2\n"); }
#define SW_FRAME 56
static void sw_prime(uint64_t *f, int64_t k, void (*entry)(int64_t)) {
    f[3] = (uint64_t)(uintptr_t)entry;
    f[4] = (uint64_t)k;
    f[5] = 0;
    f[6] = (uint64_t)(uintptr_t)sw_boot;
}
#else
#error "sw.h: AArch64 and x86-64 only"
#endif
static void sw_entry(int64_t k) {
    sw_out[k] = sw_fn[k](k);
    for (;;) sw_swap(&sw_sp[k], sw_main_sp);
}
/* Starts coroutine k running f(k) until it yields; its stack is used again when k is started again. */
static inline void sw_start(int64_t k, sw_fn_t f) {
    sw_fn[k] = f;
    if (sw_carved == NULL && sw_heap[k] == NULL) sw_heap[k] = malloc(SW_STACK);
    char *base = sw_carved != NULL ? sw_carved + k * SW_STACK : (char *)sw_heap[k];
    char *top = (char *)(((uintptr_t)(base + SW_STACK)) & ~(uintptr_t)15);
    uint64_t *fr = (uint64_t *)(void *)(top - SW_FRAME);
    for (int i = 0; i < SW_FRAME / 8; i++) fr[i] = 0;
    sw_prime(fr, k, sw_entry);
    sw_sp[k] = fr;
    sw_now = (int)k;
    sw_swap(&sw_main_sp, sw_sp[k]);
}
/* Back to whoever started or resumed the running coroutine. */
static inline void sw_yield(void) { sw_swap(&sw_sp[sw_now], sw_main_sp); }
/* Resumes coroutine k until it yields or ends; its answer once it ended. */
static inline int64_t sw_resume(int64_t k) {
    sw_now = (int)k;
    sw_swap(&sw_main_sp, sw_sp[k]);
    return sw_out[k];
}
/* Runs body(n) with the coroutines' stacks taken from an array in this function's frame. */
static inline int64_t sw_host(sw_fn_t body, int64_t n) {
    char stacks[SW_N * SW_STACK];
    sw_carved = stacks;
    int64_t r = body(n);
    sw_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return r;
}

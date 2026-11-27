/* Panel 255, third reading, the critic: UB, the tree's inline jump out of a
 * handler (`fixedbugs-2522-an-inline-jump-out-of-a-handler.h`, whose machine
 * part is copied below unchanged) with the program called SHALLOW, from the
 * runner's frame, above the handler's (raised 16 KiB below the runner), or
 * deep, 256 KiB below as the tree's case. The handler is left for good by a
 * jump no function of the C library makes: correct; the tree pins the deep
 * form's refusal (ruling B); the shallow form is what the engineer names by
 * reading as C4's cost against today. arm64 and x86-64 only. */
#ifndef UB_255_H
#define UB_255_H
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#if defined(__x86_64__)
typedef void *inline_buf[5];
#define INLINE_SAVE(b) __builtin_setjmp(b)
#define INLINE_JUMP(b) __builtin_longjmp(b, 1)
#elif defined(__aarch64__)
/* x19 to x28, the frame record x29 and x30, the stack pointer, d8 to d15. */
typedef uint64_t inline_buf[21];
__attribute__((naked, noinline, returns_twice)) static int inline_save(inline_buf b) {
    __asm__ volatile("stp x19, x20, [x0, #0]\n\t"
                     "stp x21, x22, [x0, #16]\n\t"
                     "stp x23, x24, [x0, #32]\n\t"
                     "stp x25, x26, [x0, #48]\n\t"
                     "stp x27, x28, [x0, #64]\n\t"
                     "stp x29, x30, [x0, #80]\n\t"
                     "mov x9, sp\n\t"
                     "str x9, [x0, #96]\n\t"
                     "stp d8, d9, [x0, #104]\n\t"
                     "stp d10, d11, [x0, #120]\n\t"
                     "stp d12, d13, [x0, #136]\n\t"
                     "stp d14, d15, [x0, #152]\n\t"
                     "mov w0, #0\n\t"
                     "ret");
}
/* Naked and NOT `noreturn`: under UndefinedBehaviorSanitizer clang instruments
 * the end of a `noreturn` function with a call, and at `-O2` it set that call's
 * argument in x0 before the assembly, which then read its buffer from the wrong
 * place (SIGILL in Linux arm64 under `--sanitize -O2`, 2026-11-20, read in its
 * disassembly); so the stack unpoisoning AddressSanitizer gives a `noreturn`
 * call is asked by hand before the jump (`INLINE_JUMP`). */
__attribute__((naked, noinline)) static void inline_jump(inline_buf b) {
    __asm__ volatile("ldp x19, x20, [x0, #0]\n\t"
                     "ldp x21, x22, [x0, #16]\n\t"
                     "ldp x23, x24, [x0, #32]\n\t"
                     "ldp x25, x26, [x0, #48]\n\t"
                     "ldp x27, x28, [x0, #64]\n\t"
                     "ldp x29, x30, [x0, #80]\n\t"
                     "ldr x9, [x0, #96]\n\t"
                     "mov sp, x9\n\t"
                     "ldp d8, d9, [x0, #104]\n\t"
                     "ldp d10, d11, [x0, #120]\n\t"
                     "ldp d12, d13, [x0, #136]\n\t"
                     "ldp d14, d15, [x0, #152]\n\t"
                     "mov w0, #1\n\t"
                     "ret");
}
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
void __asan_handle_no_return(void);
#define INLINE_UNPOISON() __asan_handle_no_return()
#endif
#endif
#if !defined(INLINE_UNPOISON)
#define INLINE_UNPOISON() ((void)0)
#endif
#define INLINE_SAVE(b) inline_save(b)
#define INLINE_JUMP(b) (INLINE_UNPOISON(), inline_jump(b))
#else
#error this case names no jump of its own for this machine
#endif

static inline_buf inline_env;
static void ub_handler(int sig) {
    (void)sig;
    INLINE_JUMP(inline_env);
}
__attribute__((noinline)) static int32_t ub_escape(void) {
    volatile char pad[16384];
    pad[0] = 1;
    __asm__ volatile("" ::"r"(pad) : "memory"); /* kept whole at -O2 (step 199p) */
    if (INLINE_SAVE(inline_env) == 0) {
        raise(SIGUSR1);
        return 0;
    }
    return 1;
}
__attribute__((noinline)) static int64_t ub_deep(int64_t (*cb)(int64_t)) {
    char room[1 << 18];
    memset(room, 1, sizeof room);
    __asm__ volatile("" : : "r"(room) : "memory");
    return cb(82) + room[0] - room[sizeof room - 1];
}
__attribute__((noinline)) static int64_t ub_run(int32_t deep, int32_t onstack, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = ub_handler;
    sa.sa_flags = SA_NODEFER | (onstack ? SA_ONSTACK : 0);
    sigemptyset(&sa.sa_mask);
    if (onstack) {
        size_t size = (size_t)256 << 10;
        void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
        if (alt == MAP_FAILED || sigaltstack(&ss, NULL) != 0) return -1;
    }
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    if (ub_escape() != 1) return -3;
    return deep ? ub_deep(cb) : cb(81);
}
#endif

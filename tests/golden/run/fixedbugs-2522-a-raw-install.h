/* Defect 2522 (lane-signals-2, 2026-11-20): a handler installed by a raw system
 * call, which passes through no name the runtime defines or rewrites, so no
 * route of the runtime sees the handler begin (the residue
 * `runtime/parts/siginstall.c` and `runtime/parts/rebind.c` tell): Linux's
 * `rt_sigaction` asked by `syscall`, with a restorer of this file's own on
 * x86-64, where the kernel needs one; and Darwin's `__sigaction`,
 * libsystem_kernel's, handed a trampoline of this file's own on the kernel's
 * contract, the one `rebind.c` follows (the kernel calls it with the catcher,
 * the style, the signal, its information, the context and a token, and it
 * returns by `__sigreturn(context, 30, token)` under the install's flag
 * 0x400), since libSystem's own `_sigtramp` can be neither looked up nor
 * linked (measured that day). Its frame is neither type-checked nor
 * instrumented, for `rebind.c`'s reasons. Included by the program's own
 * headers; POSIX's, whose includers ask `unistd.h` first. */
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#if defined(__APPLE__)
struct raw_install_action {
    void (*handler)(int);
    void (*tramp)(void *, int, int, siginfo_t *, void *);
    sigset_t mask;
    int flags;
};
extern int __sigaction(int, struct raw_install_action *, struct sigaction *);
extern int __sigreturn(void *, int, uintptr_t);
__attribute__((no_sanitize("function", "address"))) static void raw_install_tramp(
    void (*catcher)(int, siginfo_t *, void *), int style, int sig, siginfo_t *info, void *context, uintptr_t token) {
    (void)style;
    catcher(sig, info, context);
    __sigreturn(context, 30, token);
    __builtin_trap();
}
static inline int raw_install(int sig, void (*handler)(int)) {
    struct raw_install_action a;
    memset(&a, 0, sizeof a);
    a.handler = handler;
    a.tramp = (void (*)(void *, int, int, siginfo_t *, void *))(void *)raw_install_tramp;
    a.flags = 0x400;
    return __sigaction(sig, &a, NULL);
}
#else
#include <sys/syscall.h>
struct raw_install_action {
    void (*handler)(int);
    unsigned long flags;
    void (*restorer)(void);
    uint64_t mask;
};
#if defined(__x86_64__)
__attribute__((naked)) static void raw_install_restorer(void) { __asm__ volatile("mov $15, %eax\n\tsyscall"); }
#define RAW_INSTALL_FLAGS 0x04000000UL
#else
#define RAW_INSTALL_FLAGS 0UL
#endif
static inline int raw_install(int sig, void (*handler)(int)) {
    struct raw_install_action a;
    memset(&a, 0, sizeof a);
    a.handler = handler;
    a.flags = RAW_INSTALL_FLAGS;
#if defined(__x86_64__)
    a.restorer = raw_install_restorer;
#endif
    return (int)syscall(SYS_rt_sigaction, sig, &a, NULL, sizeof a.mask);
}
#endif

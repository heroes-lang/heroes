/* A C library that RECOVERS from its own abort and from its own trap, on every
 * platform, and the runtime must leave no line above a process that goes on.
 *
 * POSIX: handlers for SIGABRT, SIGTRAP and SIGILL installed before `main` that
 * jump back out. The Heroes runtime installs its own in `main`, finds these and
 * calls them first, and they never return.
 *
 * Windows: a `signal` handler for SIGABRT that jumps back out of `abort`, which
 * the runtime's own `signal` handler calls first; and a `__try` around the trap,
 * which runs before the runtime's filter of last resort and so leaves it nothing
 * to see. The trap is in a function of its own, because clang's `__try` catches
 * what a CALL raises. */
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
static __attribute__((noinline)) void trap_now(void) { __builtin_trap(); }
#if defined(_WIN32)
static jmp_buf recover_jump;
static void __cdecl recover(int sig) { (void)sig; longjmp(recover_jump, 1); }
__attribute__((constructor)) static void install_recover(void) { signal(SIGABRT, recover); }
static __attribute__((noinline)) int try_abort(void) {
    if (setjmp(recover_jump) == 0) abort();
    return 1;
}
static __attribute__((noinline)) int try_trap(void) {
    __try {
        trap_now();
    } __except (1) {
        return 1;
    }
    return 0;
}
#else
#include <string.h>
static sigjmp_buf recover_jump;
static void recover(int sig) { (void)sig; siglongjmp(recover_jump, 1); }
__attribute__((constructor)) static void install_recover(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = recover;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGTRAP, &sa, NULL);
    sigaction(SIGILL, &sa, NULL);
}
static __attribute__((noinline)) int try_abort(void) {
    if (sigsetjmp(recover_jump, 1) == 0) abort();
    return 1;
}
static __attribute__((noinline)) int try_trap(void) {
    if (sigsetjmp(recover_jump, 1) == 0) trap_now();
    return 1;
}
#endif

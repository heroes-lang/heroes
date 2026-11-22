/* Panel 255, the ffi-pragmatist: ijv.c's HOW 3 made correct and finished. A
 * SIGUSR1 handler sigsetjmps, raises SIGUSR2, whose handler siglongjmps back
 * into the still-running SIGUSR1 handler; that handler then siglongjmps OUT to
 * main's buffer, and main, both handlers left, calls the program back through
 * twelve frames. On the alternate stack too (`onstack` 1). */
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static int64_t (*ns_cb)(int64_t);
static sigjmp_buf ns_out, ns_in;
static void ns_inner(int sig) { (void)sig; siglongjmp(ns_in, 1); }
static void ns_outer(int sig) {
    (void)sig;
    if (sigsetjmp(ns_in, 1) == 0) raise(SIGUSR2);
    siglongjmp(ns_out, 1);
}
__attribute__((noinline)) static int64_t ns_down(int n, int64_t v) {
    if (n == 0) return ns_cb(v);
    int64_t r = ns_down(n - 1, v);
    __asm__ volatile("" ::: "memory");
    return r;
}
static inline int64_t ns_run(int32_t onstack, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    ns_cb = cb;
    if (onstack) {
        stack_t ss;
        ss.ss_sp = malloc((size_t)1 << 18);
        ss.ss_size = (size_t)1 << 18;
        ss.ss_flags = 0;
        if (ss.ss_sp == NULL || sigaltstack(&ss, NULL) != 0) return -1;
    }
    memset(&sa, 0, sizeof sa);
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = onstack ? SA_ONSTACK : 0;
    sa.sa_handler = ns_inner;
    sigaction(SIGUSR2, &sa, NULL);
    sa.sa_handler = ns_outer;
    sigaction(SIGUSR1, &sa, NULL);
    if (sigsetjmp(ns_out, 1) == 0) {
        raise(SIGUSR1);
        return -2;
    }
    return ns_down(12, 7);
}

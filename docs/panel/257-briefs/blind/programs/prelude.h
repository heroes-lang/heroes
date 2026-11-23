/* Ordinary C helpers every program below shares; it compiles as it stands. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*cb_t)(int64_t);

/* Install `h` as the handler of `sig`. */
static int on(int sig, void (*h)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    return sigaction(sig, &sa, NULL);
}

/* Call `cb` from a frame 64 KiB deeper than the caller's, no signal running. */
__attribute__((noinline)) static int64_t call(cb_t cb) {
    volatile char *pad = __builtin_alloca(64u << 10);
    pad[0] = 1;
    int64_t r = cb(7);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return r;
}

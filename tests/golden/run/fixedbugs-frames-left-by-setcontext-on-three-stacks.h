/* Frames with arrays left by `setcontext`, then frames of another layout
 * run where they stood: on the thread's stack (2), on a coroutine's heap stack
 * (4), and on an alternate signal stack, a handler leaving by `setcontext`
 * (6). Correct; under `--sanitize` nothing may be written on stderr, neither
 * AddressSanitizer's warning that it ignored a call on a stack it does not
 * know nor a report from the redzones of a frame that is gone. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
static int64_t (*kp_cb)(int64_t);
static ucontext_t *kp_main, *kp_co, *kp_back;
static volatile int kp_phase, kp_round;
static volatile int64_t kp_got;
static void kp_leave(int how) {
    if (how == 2) setcontext(kp_back);
}
__attribute__((noinline)) static int64_t kp_deep(int n, int how) {
    volatile char a[40];
    volatile int64_t b[3];
    for (int i = 0; i < 40; i++) a[i] = (char)(n + i);
    b[0] = n;
    b[2] = a[39];
    if (n == 0) kp_leave(how);
    int64_t r = kp_deep(n - 1, how);
    return r + a[5] + b[2];
}
__attribute__((noinline)) static int64_t kp_after(int n) {
    volatile char c[300];
    for (int i = 0; i < 300; i++) c[i] = (char)(i & 63);
    int64_t s = c[7] + c[299];
    if (n > 0) s += kp_after(n - 1);
    return s;
}
static void kp_body4(void) {
    kp_phase = 0;
    getcontext(kp_back);
    if (kp_phase == 0) {
        kp_phase = 1;
        kp_deep(8, 2);
        return;
    }
    kp_got = kp_after(8) + kp_cb(4);
}
__attribute__((noinline)) static int64_t kp_two(void) {
    kp_phase = 0;
    getcontext(kp_back);
    if (kp_phase == 0) {
        kp_phase = 1;
        kp_deep(8, 2);
        return -9;
    }
    return kp_after(8) + kp_cb(2);
}
static void kp_handler6(int sig) {
    (void)sig;
    if (kp_round == 0) kp_deep(6, 2);
    kp_got += kp_after(4);
    setcontext(kp_back);
}
static inline int64_t kp_run(int32_t which, int64_t (*cb)(int64_t)) {
    kp_cb = cb;
    kp_got = 0;
    kp_main = calloc(1, sizeof(ucontext_t));
    kp_co = calloc(1, sizeof(ucontext_t));
    kp_back = calloc(1, sizeof(ucontext_t));
    if (kp_main == NULL || kp_co == NULL || kp_back == NULL) return -1;
    if (which == 4) {
        void *stack = malloc((size_t)1 << 18);
        if (stack == NULL || getcontext(kp_co) != 0) return -1;
        kp_co->uc_stack.ss_sp = stack;
        kp_co->uc_stack.ss_size = (size_t)1 << 18;
        kp_co->uc_link = kp_main;
        makecontext(kp_co, kp_body4, 0);
        swapcontext(kp_main, kp_co);
        return kp_got;
    }
    if (which == 2) return kp_two();
    size_t size = (size_t)256 << 10;
    void *alt = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    stack_t ss = {.ss_sp = alt, .ss_size = size, .ss_flags = 0};
    if (alt == MAP_FAILED || sigaltstack(&ss, NULL) != 0) return -2;
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = kp_handler6;
    sa.sa_flags = SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -3;
    for (kp_round = 0; kp_round < 3; kp_round = kp_round + 1) {
        kp_phase = 0;
        getcontext(kp_back);
        if (kp_phase == 0) {
            kp_phase = 1;
            raise(SIGUSR1);
        }
    }
    return kp_got + kp_cb(which);
}

/* Defect 2868 (lane sig-family-3): that a coroutine's end is seen. A
 * helper runs a coroutine on an array of its own frame to its end through
 * `uc_link` (`how` 0: entered by `swapcontext`; 1: by `setcontext`, its
 * `uc_link` the helper's `getcontext`), and returns; the thread then saves a
 * point in frames lying where that array was, a signal comes below that array (200 KiB down), and the
 * handler saves its own point, then jumps back to the save point; the program
 * is called after. The jump goes back into the call the signal interrupted,
 * which ends the handler (panel 262's R1): MUST RUN. Where the coroutine's
 * end is not seen, its range still reads as a coroutine's, and the handler,
 * holding a save, is held parked (ONSTACK's words). */
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

static ucontext_t re_main, *re_task;
static volatile int re_done;
static sigjmp_buf re_env, re_hb;
static void re_body(void) { re_done = 1; }

__attribute__((noinline)) static void re_helper(int how) {
    char stack[1u << 18];
    stack[0] = 1;
    re_task = calloc(1, sizeof(ucontext_t));
    getcontext(re_task);
    re_task->uc_stack.ss_sp = stack;
    re_task->uc_stack.ss_size = sizeof stack;
    re_task->uc_link = &re_main;
    makecontext(re_task, re_body, 0);
    if (how == 0) {
        swapcontext(&re_main, re_task);
    } else {
        volatile int back = 0;
        getcontext(&re_main);
        if (!back) {
            back = 1;
            setcontext(re_task);
        }
    }
    __asm__ volatile("" ::"r"(stack) : "memory");
}

static void re_handler(int sig) {
    (void)sig;
    if (sigsetjmp(re_hb, 0) != 0) return;
    siglongjmp(re_env, 1);
}

__attribute__((noinline)) static void re_descend(int64_t k) {
    volatile char pad[2048];
    pad[0] = 1;
    __asm__ volatile("" ::"r"(pad) : "memory");
    if (k > 0) re_descend(k - 1); else raise(SIGUSR1);
}

__attribute__((noinline)) static int64_t re_deep(int64_t depth, int64_t (*cb)(int64_t)) {
    volatile char pad[2048];
    pad[0] = 1;
    __asm__ volatile("" ::"r"(pad) : "memory");
    if (depth > 0) return re_deep(depth - 1, cb) + (int64_t)(pad[0] * 0);
    if (sigsetjmp(re_env, 1) == 0) {
        re_descend(100);
        return -3;
    }
    return cb(7);
}

static inline int64_t re_run(int32_t how, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = re_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    re_helper(how);
    free(re_task);
    if (!re_done) return -4;
    return re_deep(40, cb);
}

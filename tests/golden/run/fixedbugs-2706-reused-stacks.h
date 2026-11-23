/* fixedbugs-2706-reused-stacks.h: a CORRECT `siglongjmp` out of a signal handler onto the thread's own
 * stack, in frames that occupy memory once handed to `makecontext`. A helper hands `makecontext` a local array of
 * its own frame and returns, and the array is dead: its coroutine either finished (rs_ended: the task returned
 * through `uc_link` before the helper did) or was never switched to (rs_unused). Later the SAME thread, in frames
 * that now occupy that memory, saves a point with `sigsetjmp`, takes a signal raised `down` frames of 2 KiB below
 * it, and the handler `siglongjmp`s back to the save point; the program is then called. `depth` frames of 2 KiB
 * put the save point inside the dead array, `down` frames below it put the signal past the array's low end
 * (panel 257's critic's `n6` and the ffi-pragmatist's `fp3`: parameterised, silent about addresses). */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <ucontext.h>

static ucontext_t rs_main, rs_task;
static volatile int rs_done;
static void rs_body(void) { rs_done = 1; }

__attribute__((noinline)) static void rs_helper(int run) {
    char stack[1u << 18];
    stack[0] = 1;
    getcontext(&rs_task);
    rs_task.uc_stack.ss_sp = stack;
    rs_task.uc_stack.ss_size = sizeof stack;
    rs_task.uc_link = &rs_main;
    makecontext(&rs_task, rs_body, 0);
    if (run) swapcontext(&rs_main, &rs_task);
    __asm__ volatile("" ::"r"(stack) : "memory");
}

static sigjmp_buf rs_env;
static void rs_handler(int sig) {
    (void)sig;
    siglongjmp(rs_env, 1);
}

__attribute__((noinline)) static void rs_descend(int64_t k) {
    volatile char pad[2048];
    pad[0] = 1;
    if (k > 0) rs_descend(k - 1); else raise(SIGUSR1);
    __asm__ volatile("" ::"r"(pad) : "memory");
}

__attribute__((noinline)) static int64_t rs_deep(int64_t depth, int64_t down, int64_t (*cb)(int64_t)) {
    volatile char pad[2048];
    pad[0] = 1;
    if (depth > 0) {
        int64_t r = rs_deep(depth - 1, down, cb);
        return r + (int64_t)(pad[0] * 0);
    }
    if (sigsetjmp(rs_env, 1) == 0) {
        rs_descend(down);
        return -3;
    }
    return cb(7);
}

static inline int64_t rs_case(int64_t run, int64_t depth, int64_t down, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = rs_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    rs_helper((int)run);
    if (run && !rs_done) return -4;
    return rs_deep(depth, down, cb);
}

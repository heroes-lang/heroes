/* fixedbugs-a-coroutine-that-yields-by-saving-into-another-context.h: a coroutine the runner switched to with one
 * context saves a buffer with `sigsetjmp`, then yields by a `swapcontext` that saves into ANOTHER context, so the
 * context it was switched to comes back unchanged though the coroutine lives. A signal's handler on the thread's
 * own stack then `siglongjmp`s into the coroutine through that buffer, and the coroutine calls the program inside
 * the signal's extent, then jumps back to the runner. `stack` 0 puts the coroutine on an array of the runner's live
 * frame (the thread's own stack), 1 on the heap. */
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

typedef int64_t (*ye_cb)(int64_t);
static ucontext_t ye_main, ye_co, ye_other;
static sigjmp_buf ye_kb, ye_back;
static ye_cb ye_f;
static volatile int64_t ye_got = -7;

static void ye_body(void) {
    if (sigsetjmp(ye_kb, 0) == 0) swapcontext(&ye_other, &ye_main);
    ye_got = ye_f(4);
    siglongjmp(ye_back, 1);
}

static void ye_handler(int sig) {
    (void)sig;
    siglongjmp(ye_kb, 1);
}

__attribute__((noinline)) static int64_t ye_run(int32_t stack, ye_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    void *heap = stack == 1 ? malloc((size_t)1 << 18) : NULL;
    ye_f = cb;
    if ((stack == 1 && heap == NULL) || getcontext(&ye_co) != 0) return -1;
    ye_co.uc_stack.ss_sp = stack == 1 ? heap : (void *)local;
    ye_co.uc_stack.ss_size = stack == 1 ? (size_t)1 << 18 : sizeof local;
    ye_co.uc_link = &ye_main;
    makecontext(&ye_co, ye_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = ye_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    swapcontext(&ye_main, &ye_co);
    if (sigsetjmp(ye_back, 1) == 0) raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    free(heap);
    return ye_got;
}

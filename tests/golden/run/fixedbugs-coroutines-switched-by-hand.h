/* fixedbugs-coroutines-switched-by-hand.h: up to twenty coroutines, each running a function of the program on a
 * stack of its own, switched by `swapcontext`; the stacks are the heap's, or the first four carved from the thread's
 * own stack (cr_host). The shape of the defect lane panel258-r1 found beside defect 2752 (`co_room`).
 * UNDER AddressSanitizer each switch is told to it as a fiber's (`__sanitizer_start_switch_fiber`), as a coroutine
 * library's must be: a `longjmp` on a coroutine's stack otherwise has it warn on stderr that it ignores the jump's
 * unpoisoning (`run/fixedbugs-a-c-longjmp-over-a-callback-inside-a-coroutine-gives-its-room-back`, measured
 * 2026-11-25). And a case starts each coroutine on a slot of its own but where it means to use a stack again: a
 * stack used again by frames laid out otherwise read its stale poison as an underflow on Darwin (measured the same
 * day, two coroutines started twice on the same two stacks, at `--sanitize`). */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <ucontext.h>
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
void __sanitizer_start_switch_fiber(void **fake_stack_save, const void *bottom, size_t size);
void __sanitizer_finish_switch_fiber(void *fake_stack_save, const void **bottom_old, size_t *size_old);
#define CR_FIBERS 1
#endif
#endif
typedef int64_t (*cr_cb_t)(int64_t);
#define CR_N 20
#define CR_CARVED 4
#define CR_STACK (256u << 10)
static ucontext_t cr_main, cr_ctx[CR_N];
static cr_cb_t cr_fn[CR_N];
static int64_t cr_out[CR_N];
static int cr_now;
static char *cr_carved;
static void *cr_heap[CR_N];
static char *cr_bottom[CR_N];
/* The sanitizer's halves of a switch: before it, the stack switched to (`bottom` NULL for the caller's, learned at
 * the coroutine's first entry); after it, back on this stack. Nothing without the sanitizer. */
#if defined(CR_FIBERS)
static const void *cr_main_bottom;
static size_t cr_main_size;
static void *cr_fake_main, *cr_fake[CR_N];
static void cr_to(void **save, int to) {
    if (to < 0) __sanitizer_start_switch_fiber(save, cr_main_bottom, cr_main_size);
    else __sanitizer_start_switch_fiber(save, cr_bottom[to], CR_STACK);
}
static void cr_back(void *save) { __sanitizer_finish_switch_fiber(save, &cr_main_bottom, &cr_main_size); }
static void cr_back_main(void *save) { __sanitizer_finish_switch_fiber(save, NULL, NULL); }
#define CR_SAVE_MAIN (&cr_fake_main)
#define CR_SAVE(k) (&cr_fake[k])
#define CR_FAKE_MAIN cr_fake_main
#define CR_FAKE(k) cr_fake[k]
#else
#define cr_to(save, to) ((void)0)
#define cr_back(save) ((void)0)
#define cr_back_main(save) ((void)0)
#define CR_SAVE_MAIN NULL
#define CR_SAVE(k) NULL
#define CR_FAKE_MAIN NULL
#define CR_FAKE(k) NULL
#endif
static void cr_entry(int k) {
    cr_back(NULL);
    cr_out[k] = cr_fn[k](k);
    cr_to(NULL, -1);
    swapcontext(&cr_ctx[k], &cr_main);
}
/* Starts coroutine k running f(k) until it yields; its stack is reused when k is started again. */
static inline void cr_start(int64_t k, cr_cb_t f) {
    cr_fn[k] = f;
    getcontext(&cr_ctx[k]);
    if (cr_carved == NULL && cr_heap[k] == NULL) cr_heap[k] = malloc(CR_STACK);
    cr_ctx[k].uc_stack.ss_size = CR_STACK;
    cr_bottom[k] = cr_carved != NULL ? cr_carved + k * CR_STACK : (char *)cr_heap[k];
    cr_ctx[k].uc_stack.ss_sp = cr_bottom[k];
    cr_ctx[k].uc_link = &cr_main;
    makecontext(&cr_ctx[k], (void (*)(void))cr_entry, 1, (int)k);
    cr_now = (int)k;
    cr_to(CR_SAVE_MAIN, (int)k);
    swapcontext(&cr_main, &cr_ctx[k]);
    cr_back_main(CR_FAKE_MAIN);
}
/* Back to the caller of cr_start or cr_resume from the coroutine running. */
static inline void cr_yield(void) {
    int k = cr_now;
    cr_to(CR_SAVE(k), -1);
    swapcontext(&cr_ctx[k], &cr_main);
    cr_back(CR_FAKE(k));
}
/* Resumes coroutine k until it yields or ends; its answer once it ended. */
static inline int64_t cr_resume(int64_t k) {
    cr_now = (int)k;
    cr_to(CR_SAVE_MAIN, (int)k);
    swapcontext(&cr_main, &cr_ctx[k]);
    cr_back_main(CR_FAKE_MAIN);
    return cr_out[k];
}
/* Leaves the coroutine running for good, saving nothing: its frames never finish. */
static inline void cr_quit(void) {
    cr_to(NULL, -1);
    setcontext(&cr_main);
}
/* Runs body with the coroutines' stacks carved from this frame, on the thread's own stack. */
static inline int64_t cr_host(cr_cb_t body, int64_t n) {
    char stacks[CR_CARVED * CR_STACK];
    cr_carved = stacks;
    int64_t r = body(n);
    cr_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return r;
}
/* C adds one to *n, the local lent to it in place. */
static inline void cr_bump(int64_t *n) { *n += 1; }
/* C writes k into the buffer lent to it, yields, and writes k + 1 beside it once resumed. */
static inline void cr_yield_lent(int64_t k, unsigned char *buf) {
    buf[0] = (unsigned char)k;
    cr_yield();
    buf[1] = (unsigned char)(k + 1);
}

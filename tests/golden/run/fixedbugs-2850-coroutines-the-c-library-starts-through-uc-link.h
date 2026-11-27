/* Defect 2850 (lane sig-family): coroutines the C library starts through
 * another's `uc_link`, entered by no switch the program makes. Portable:
 * Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`).
 * ul_run form 0: A (heap) ends, its `uc_link` names B (an array of the
 *   runner's frame), so the C library starts B; B saves a buffer and yields
 *   by `swapcontext`; a handler jumps into B, which calls the program:
 *   MUST BE REFUSED.
 * form 1: the same with B on the heap: MUST BE REFUSED.
 * form 2: A made before B (A's `uc_link` names a context not yet made): MUST
 *   BE REFUSED.
 * form 3: a chain, A (heap) to B (heap) to C (the runner's array); C saves,
 *   yields, and is jumped into: MUST BE REFUSED.
 * ul_reuse: a helper makes A (heap) and B (an array of its own frame), A's
 *   `uc_link` B and B's the helper's, runs both to their end and returns;
 *   later the thread saves a point in frames that occupy B's dead array, a
 *   signal comes below it, and the handler jumps back to the save point; the
 *   program is called after: correct, MUST RUN. */
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

typedef int64_t (*ul_cb)(int64_t);
static ucontext_t ul_main, ul_a, ul_b, ul_c, ul_y;
static sigjmp_buf ul_kb, ul_back;
static ul_cb ul_f;
static volatile int64_t ul_got = -7;

static void ul_body_a(void) {}
static void ul_body_b(void) {}

static void ul_body_last(void) {
    if (sigsetjmp(ul_kb, 0) == 0) swapcontext(&ul_y, &ul_main);
    ul_got = ul_f(4);
    siglongjmp(ul_back, 1);
}

static void ul_handler(int sig) {
    (void)sig;
    siglongjmp(ul_kb, 1);
}

static int ul_on(void (*h)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    return sigaction(SIGUSR1, &sa, NULL);
}

static int ul_make(ucontext_t *uc, void *stack, size_t size, ucontext_t *link, void (*body)(void)) {
    if (stack == NULL || getcontext(uc) != 0) return -1;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = size;
    uc->uc_link = link;
    makecontext(uc, body, 0);
    return 0;
}

__attribute__((noinline)) static int64_t ul_run(int32_t form, ul_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    size_t big = (size_t)1 << 18;
    void *h1 = malloc(big), *h2 = malloc(big);
    ul_f = cb;
    int bad = 0;
    if (form == 0) {
        bad |= ul_make(&ul_b, local, sizeof local, &ul_main, ul_body_last);
        bad |= ul_make(&ul_a, h1, big, &ul_b, ul_body_a);
    } else if (form == 1) {
        bad |= ul_make(&ul_b, h2, big, &ul_main, ul_body_last);
        bad |= ul_make(&ul_a, h1, big, &ul_b, ul_body_a);
    } else if (form == 2) {
        bad |= ul_make(&ul_a, h1, big, &ul_b, ul_body_a);
        bad |= ul_make(&ul_b, local, sizeof local, &ul_main, ul_body_last);
    } else {
        bad |= ul_make(&ul_c, local, sizeof local, &ul_main, ul_body_last);
        bad |= ul_make(&ul_b, h2, big, &ul_c, ul_body_b);
        bad |= ul_make(&ul_a, h1, big, &ul_b, ul_body_a);
    }
    if (bad != 0) return -1;
    if (ul_on(ul_handler) != 0) return -2;
    swapcontext(&ul_main, &ul_a);
    if (sigsetjmp(ul_back, 1) == 0) raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    free(h1);
    free(h2);
    return ul_got;
}

static ucontext_t ur_main, ur_a, ur_b;
static volatile int ur_done;
static void ur_body_a(void) {}
static void ur_body_b(void) { ur_done = 1; }

__attribute__((noinline)) static void ur_helper(void *heap) {
    char stack[1u << 18];
    stack[0] = 1;
    ul_make(&ur_b, stack, sizeof stack, &ur_main, ur_body_b);
    ul_make(&ur_a, heap, (size_t)1 << 18, &ur_b, ur_body_a);
    swapcontext(&ur_main, &ur_a);
    __asm__ volatile("" ::"r"(stack) : "memory");
}

static sigjmp_buf ur_env;
static void ur_handler(int sig) {
    (void)sig;
    siglongjmp(ur_env, 1);
}

__attribute__((noinline)) static void ur_descend(int64_t k) {
    volatile char pad[2048];
    pad[0] = 1;
    if (k > 0) ur_descend(k - 1); else raise(SIGUSR1);
    __asm__ volatile("" ::"r"(pad) : "memory");
}

__attribute__((noinline)) static int64_t ur_deep(int64_t depth, int64_t down, ul_cb cb) {
    volatile char pad[2048];
    pad[0] = 1;
    __asm__ volatile("" ::"r"(pad) : "memory"); /* kept whole at -O2 (step 199p) */
    if (depth > 0) {
        int64_t r = ur_deep(depth - 1, down, cb);
        return r + (int64_t)(pad[0] * 0);
    }
    if (sigsetjmp(ur_env, 1) == 0) {
        ur_descend(down);
        return -3;
    }
    return cb(7);
}

static inline int64_t ul_reuse(ul_cb cb) {
    void *heap = malloc((size_t)1 << 18);
    if (heap == NULL || ul_on(ur_handler) != 0) return -2;
    ur_helper(heap);
    free(heap);
    if (!ur_done) return -4;
    return ur_deep(5, 150, cb);
}

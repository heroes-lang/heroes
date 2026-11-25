/* fixedbugs-a-coroutine-gives-back-the-context-it-was-switched-to.h: a coroutine gives back the context the runner
 * switched to it with, at the first moment C allows (the coroutine runs only after the C library has read that
 * context), then either yields by a `swapcontext` that saves into ANOTHER context, which the runner never resumes
 * (`ends` 0), or ends through `uc_link` (`ends` 1). How the context is given back: 0 its page unmapped, 1 its heap
 * block freed, 2 its page made unreadable with `mprotect` (the runner unmaps it once the switch returns). Correct
 * C: nothing reads the context again. On Darwin a coroutine that ends gives nothing back, and the runner gives the
 * context back once the switch has returned: Darwin's C library reads the context again when the coroutine returns
 * (`_ctx_done` faulted on the unmapped page, measured 2026-11-25), so there the program would not be correct, and
 * it says the same answer. */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ucontext.h>

static ucontext_t gb_main;
static ucontext_t *gb_co;
static ucontext_t *gb_other;
static size_t gb_size;
static int gb_how, gb_ends;

static void gb_give_back(void) {
#if defined(__APPLE__)
    if (gb_ends) return;
#endif
    if (gb_how == 0) {
        munmap(gb_co, gb_size);
    } else if (gb_how == 1) {
        free(gb_co);
    } else {
        mprotect(gb_co, gb_size, PROT_NONE);
    }
}

static void gb_body(void) {
    gb_give_back();
    if (!gb_ends) swapcontext(gb_other, &gb_main);
}

static inline int64_t gb_run(int32_t ends, int32_t how) {
    gb_how = how;
    gb_ends = ends;
    gb_size = (size_t)1 << 16;
    if (how == 1) {
        gb_co = calloc(1, sizeof(ucontext_t));
        if (gb_co == NULL) return -1;
    } else {
        void *page = mmap(NULL, gb_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (page == MAP_FAILED) return -1;
        gb_co = page;
    }
    gb_other = calloc(1, sizeof(ucontext_t));
    void *stack = malloc((size_t)1 << 18);
    if (stack == NULL || gb_other == NULL || getcontext(gb_co) != 0) return -2;
    gb_co->uc_stack.ss_sp = stack;
    gb_co->uc_stack.ss_size = (size_t)1 << 18;
    gb_co->uc_link = &gb_main;
    makecontext(gb_co, gb_body, 0);
    if (swapcontext(&gb_main, gb_co) != 0) return -3;
    if (how == 2) munmap(gb_co, gb_size);
#if defined(__APPLE__)
    if (gb_ends && how == 0) munmap(gb_co, gb_size);
    if (gb_ends && how == 1) free(gb_co);
#endif
    free(stack);
    free(gb_other);
    return 7;
}

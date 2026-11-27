/* co.h (panel 263's clerk, 2026-11-27): the defect d-lane-sig-family-2-1's
 * reproducer, written again from its draft's words, the lane's own `co/co.h`
 * having gone with lane sig-family-2's scratch (`ls .claude/worktrees`, no
 * `lane-sig-family-2-scratch`). A function of the program C calls back on a
 * coroutine `makecontext` started, on a stack of 256 KiB, entered by
 * `swapcontext`; the callback's answer is handed back once the coroutine
 * ends. No signal handler is installed.
 *
 * `kind` picks the stack:
 *   0  `mmap`, with one page of no access below it (as defect 2870's golden
 *      `fixedbugs-2870-a-coroutines-guarded-stack.h`);
 *   1  `malloc`, no page of no access below it: the defect;
 *   2  `mmap`, no page of no access below it, the mapping below whatever the
 *      kernel put there;
 *   3  no coroutine: the callback called on the thread's own stack (the control).
 *
 * `CO_SAY` set in the environment writes, before the switch, the stack's
 * lowest and highest address and the address of a thread-local word of this
 * header's (`co_tls_probe`) on the calling thread, so the distance from the
 * stack to this thread's thread-local storage is read, not inferred.
 *
 * Portable: Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`). */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*co_cb)(int64_t);
#define CO_STACK ((size_t)256 << 10)
static ucontext_t co_home, co_co;
static co_cb co_fn;
static int64_t co_arg, co_out;
static _Thread_local int64_t co_tls_probe = 1;

static void co_entry(void) { co_out = co_fn(co_arg); }

static inline int64_t co_run(int64_t kind, co_cb cb, int64_t n) {
    if (kind == 3) return cb(n);
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    char *stack = NULL;
    if (kind == 1) {
        stack = malloc(CO_STACK);
        if (stack == NULL) return -2;
    } else {
        size_t below = kind == 0 ? page : 0;
        char *map = mmap(NULL, CO_STACK + below, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (map == MAP_FAILED) return -2;
        if (below != 0 && mprotect(map, page, PROT_NONE) != 0) return -3;
        stack = map + below;
    }
    if (getenv("CO_SAY") != NULL) {
        fprintf(stderr, "co: kind %lld stack [%p, %p) tls %p\n", (long long)kind, (void *)stack,
                (void *)(stack + CO_STACK), (void *)&co_tls_probe);
    }
    co_fn = cb;
    co_arg = n;
    if (getcontext(&co_co) != 0) return -4;
    co_co.uc_stack.ss_sp = stack;
    co_co.uc_stack.ss_size = CO_STACK;
    co_co.uc_link = &co_home;
    makecontext(&co_co, co_entry, 0);
    if (swapcontext(&co_home, &co_co) != 0) return -5;
    return co_out;
}

/* Defect 2848 (lane sig-family): the shapes of a `setcontext` into a context
 * `makecontext` made, and of `makecontext`'s arguments, which glibc's
 * architectures other than aarch64 and x86-64 judged with no wrapper on
 * `makecontext` (none was written there until this defect: no stack it hands
 * out was known). Every route must say what this file says; the build those
 * architectures take is simulated on Linux arm64 by `HERO_JMP_MAKE_IN_C` with
 * `HERO_JMP_NO_DECODE`, run apart (a golden carries no define).
 * Portable: Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`). Stack 0 is an
 * array of the runner's live frame (the thread's own stack), 1 the heap; each
 * context is static, so a record of it can be read.
 * sm_run shape 0: a handler `setcontext`s into a context `makecontext` made;
 *     the coroutine calls the program: MUST BE REFUSED.
 * sm_run shape 1: a coroutine started by `setcontext`, which saves a buffer
 *     and comes back by `setcontext` to a context the runner took; a handler
 *     then jumps into the coroutine, which calls the program: MUST BE
 *     REFUSED.
 * sm_run shape 2: no coroutine: `getcontext` and `setcontext` used as a jump
 *     within the runner, then a save point, a signal, and the handler's jump
 *     out to it; the program is called after the handler was left: correct,
 *     MUST RUN.
 * mk_run: a coroutine `makecontext` hands twelve `int` arguments, more than
 *     the argument registers hold, so some go on the stack it makes; the
 *     coroutine weighs each by its place and hands the sum, 650, to the
 *     program; no signal: correct, MUST RUN. */
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

typedef int64_t (*sm_cb)(int64_t);
static ucontext_t sm_co, sm_home;
static sigjmp_buf sm_kb, sm_back;
static sm_cb sm_f;
static volatile int64_t sm_got = -7;
static volatile int sm_shape, sm_started;

static int sm_on(void (*h)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    return sigaction(SIGUSR1, &sa, NULL);
}

static void sm_body(void) {
    if (sm_shape == 1 && sigsetjmp(sm_kb, 0) == 0) setcontext(&sm_home);
    sm_got = sm_f(4);
    siglongjmp(sm_back, 1);
}

static void sm_to_co(int sig) {
    (void)sig;
    setcontext(&sm_co);
}

static void sm_to_kb(int sig) {
    (void)sig;
    siglongjmp(sm_kb, 1);
}

static void sm_to_back(int sig) {
    (void)sig;
    siglongjmp(sm_back, 1);
}

__attribute__((noinline)) static int64_t sm_run(int32_t shape, int32_t stack, sm_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
    void *heap = stack == 1 ? malloc((size_t)1 << 18) : NULL;
    sm_f = cb;
    sm_shape = shape;
    sm_started = 0;
    if (stack == 1 && heap == NULL) return -1;
    if (shape == 2) {
        volatile int pass = 0;
        getcontext(&sm_home);
        pass += 1;
        if (pass == 1) setcontext(&sm_home);
        if (sm_on(sm_to_back) != 0) return -2;
        if (sigsetjmp(sm_back, 1) == 0) {
            raise(SIGUSR1);
            return -3;
        }
        free(heap);
        return sm_f(5);
    }
    if (getcontext(&sm_co) != 0) return -1;
    sm_co.uc_stack.ss_sp = stack == 1 ? heap : (void *)local;
    sm_co.uc_stack.ss_size = stack == 1 ? (size_t)1 << 18 : sizeof local;
    sm_co.uc_link = NULL;
    makecontext(&sm_co, sm_body, 0);
    if (shape == 1) {
        getcontext(&sm_home);
        if (!sm_started) {
            sm_started = 1;
            setcontext(&sm_co);
        }
    }
    if (sm_on(shape == 0 ? sm_to_co : sm_to_kb) != 0) return -2;
    if (sigsetjmp(sm_back, 1) == 0) raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    free(heap);
    return sm_got;
}

typedef int64_t (*mk_cb)(int64_t);
static ucontext_t mk_main, mk_co;
static mk_cb mk_f;
static volatile int64_t mk_got = -7;
static void mk_body(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
                    int a11) {
    int64_t v[12] = {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11};
    int64_t s = 0;
    for (int i = 0; i < 12; i += 1) s += v[i] * (i + 1);
    mk_got = mk_f(s);
}
__attribute__((noinline)) static int64_t mk_run(mk_cb cb) {
    static char stack[(size_t)1 << 17];
    mk_f = cb;
    if (getcontext(&mk_co) != 0) return -1;
    mk_co.uc_stack.ss_sp = stack;
    mk_co.uc_stack.ss_size = sizeof stack;
    mk_co.uc_link = &mk_main;
    makecontext(&mk_co, (void (*)(void))(uintptr_t)mk_body, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
    if (swapcontext(&mk_main, &mk_co) != 0) return -2;
    return mk_got;
}

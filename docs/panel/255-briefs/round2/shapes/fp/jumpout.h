/* Panel 255, the ffi-pragmatist: a CORRECT jump out of a handler through a
 * buffer route A cannot prove lives (on the heap, thread-local, in a library
 * `dlopen`ed after `main`), then the program called back once the jump has
 * left the handler. And a coroutine whose context and stack are freed before
 * the next save point. Each a correct C program. */
#define _XOPEN_SOURCE 700
#include <dlfcn.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

static jmp_buf *jo_heap_env;
static _Thread_local sigjmp_buf jo_tls_env;
static sigjmp_buf *jo_lib_env;

static void jo_to_heap(int sig) { (void)sig; longjmp(*jo_heap_env, 1); }
static void jo_to_tls(int sig) { (void)sig; siglongjmp(jo_tls_env, 1); }
static void jo_to_lib(int sig) { (void)sig; siglongjmp(*jo_lib_env, 1); }

static inline void jo_install(void (*h)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = h;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
}

static inline int64_t jo_heap(int64_t (*cb)(int64_t)) {
    jo_heap_env = malloc(sizeof *jo_heap_env);
    if (jo_heap_env == NULL) return -1;
    jo_install(jo_to_heap);
    int64_t r = -2;
    if (setjmp(*jo_heap_env) == 0) {
        raise(SIGUSR1);
    } else {
        r = cb(1);
    }
    free(jo_heap_env);
    return r;
}

static inline int64_t jo_tls(int64_t (*cb)(int64_t)) {
    jo_install(jo_to_tls);
    int64_t r = -2;
    if (sigsetjmp(jo_tls_env, 1) == 0) {
        raise(SIGUSR1);
    } else {
        r = cb(2);
    }
    return r;
}

static inline int64_t jo_lib(const char *path, int64_t (*cb)(int64_t)) {
    void *h = dlopen(path, RTLD_NOW);
    if (h == NULL) return -3;
    jo_lib_env = (sigjmp_buf *)dlsym(h, "jo_lib_env");
    if (jo_lib_env == NULL) return -4;
    jo_install(jo_to_lib);
    int64_t r = -2;
    if (sigsetjmp(*jo_lib_env, 1) == 0) {
        raise(SIGUSR1);
    } else {
        r = cb(3);
    }
    return r;
}

/* The coroutine: a context and its stack on the heap, run until it yields,
 * then abandoned and both freed; then a save point and a callback. */
static ucontext_t *co_main_ctx;
static ucontext_t *co_ctx;
static void co_body(void) { swapcontext(co_ctx, co_main_ctx); }

static inline int64_t co_run(int64_t (*cb)(int64_t)) {
    co_main_ctx = calloc(1, 1024);
    co_ctx = calloc(1, 1024);
    size_t size = (size_t)1 << 16;
    void *stack = malloc(size);
    if (co_main_ctx == NULL || co_ctx == NULL || stack == NULL) return -1;
    if (getcontext(co_ctx) != 0) return -1;
    co_ctx->uc_stack.ss_sp = stack;
    co_ctx->uc_stack.ss_size = size;
    co_ctx->uc_link = co_main_ctx;
    makecontext(co_ctx, co_body, 0);
    if (swapcontext(co_main_ctx, co_ctx) != 0) return -1;
    free(stack);
    free(co_ctx);
    jmp_buf after;
    (void)setjmp(after);
    int64_t r = cb(4);
    free(co_main_ctx);
    return r;
}

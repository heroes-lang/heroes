/* Defect 2853 (lane sig-family): OpenSSL's ASYNC fibres (panel 259's
 * ffi-pragmatist's `aj2.h`), two jobs paused at once, each running a function
 * of the program on the fibre's stack; libcrypto makes a fibre with
 * `makecontext`, enters it with `setcontext` and switches after with `_setjmp`
 * and `_longjmp` on Darwin and Debian arm64 (`swapcontext` on Debian x86-64).
 * The fibres' stacks are quarters of an array in `aj_host`'s frame, through
 * `ASYNC_set_mem_functions`. A correct program: MUST RUN, under `--sanitize`
 * too. */
/* aj2.h: OpenSSL's ASYNC fibers (openssl/async.h, libcrypto 3), two jobs paused at once, each running a function of
 * the program on the fiber's stack. libcrypto makes a fiber with makecontext, enters it with setcontext and switches
 * afterwards with _setjmp/_longjmp (this Mac's `nm -u`); where the fiber's stack comes from is the program's choice,
 * through ASYNC_set_mem_functions (async.h:82-88): the library's default (OPENSSL_malloc), or, through aj_mem(),
 * quarters of an array in aj_host's frame, on the thread's own stack. The library's default stack (32 KiB in the
 * first build run, `stack-overflow ... in h_ajheap_e4` under --sanitize) is too small for these frames, so aj_mem()
 * gives every fiber 256 KiB: from malloc, or carved once aj_host runs. */
#if defined(_WIN32)
#error "OpenSSL's ASYNC fibres are made by makecontext on POSIX systems alone; Windows' are the system's own fibres"
#endif
#include <openssl/async.h>
#include <stdint.h>
#include <stdlib.h>
typedef int64_t (*aj_cb_t)(int64_t);
#define AJ_N 4
#define AJ_STACK (256u << 10)
static ASYNC_JOB *aj_job[AJ_N];
static ASYNC_WAIT_CTX *aj_wait[AJ_N];
static int aj_ret[AJ_N];
static aj_cb_t aj_fn;
static char *aj_carved;
static char *aj_base; /* the carved array, kept after aj_host returns so a stack OpenSSL frees at exit is told apart */
static int aj_used;
static int aj_ready;
static void *aj_alloc(size_t *num) {
    *num = AJ_STACK;
    if (aj_carved == NULL) return malloc(AJ_STACK);
    if (aj_used == AJ_N) return NULL;
    aj_base = aj_carved;
    return aj_carved + (aj_used++) * AJ_STACK;
}
static void aj_free(void *p) {
    if (aj_base == NULL || (char *)p < aj_base || (char *)p >= aj_base + AJ_N * AJ_STACK) free(p);
}
static int aj_job_fn(void *arg) { return (int)aj_fn(*(int64_t *)arg); }
/* Every fiber 256 KiB, the heap's or, inside aj_host, carved from its frame. Before any job. */
static inline int64_t aj_mem(void) { return ASYNC_set_mem_functions(aj_alloc, aj_free); }
static inline void aj_pause(void) { ASYNC_pause_job(); }
/* Starts or resumes job k (f runs on its fiber): 1 where it paused, 1000 + its answer where it ended, < 0 on error. */
static inline int64_t aj_go(int64_t k, aj_cb_t f) {
    if (!aj_ready) {
        if (!ASYNC_init_thread(AJ_N, 0)) return -1;
        aj_ready = 1;
    }
    aj_fn = f;
    if (aj_wait[k] == NULL) aj_wait[k] = ASYNC_WAIT_CTX_new();
    int64_t arg = k;
    int st = ASYNC_start_job(&aj_job[k], aj_wait[k], &aj_ret[k], aj_job_fn, &arg, sizeof arg);
    if (st == ASYNC_PAUSE) return 1;
    if (st == ASYNC_FINISH) return 1000 + aj_ret[k];
    return -2 - st;
}
static inline int64_t aj_host(aj_cb_t body, int64_t n) {
    char stacks[AJ_N * AJ_STACK];
    aj_carved = stacks;
    int64_t r = body(n);
    aj_carved = NULL;
    __asm__ volatile("" ::"r"(stacks) : "memory");
    return r;
}

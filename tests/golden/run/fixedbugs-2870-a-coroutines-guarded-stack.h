/* Defect 2870 (panel 262's compiler-engineer, `so`): a function of the program
 * C calls back on a coroutine `makecontext` started, on a stack of 256 KiB C
 * mapped with a page of no access below it, as a coroutine library guards
 * its stacks. `gs_run` maps the stack, makes the coroutine, switches to it,
 * and hands the callback's answer back once the coroutine ends; no signal
 * handler is installed. The callback of every case recurses past the stack,
 * so the program must stop saying `panic: stack exhausted`, as the same
 * recursion does on the thread's own stack.
 * `gs_big_first` reads one word of a record of 32 KiB the callback holds by
 * value in each of its frames, so each frame is larger than a page and is
 * probed before it is touched; the record is read through a pointer the
 * optimiser cannot see into, so its bytes exist at `-O2` as at `-O0`.
 * Portable: Darwin (`_XOPEN_SOURCE`) and glibc (`_GNU_SOURCE`). */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#include <stdint.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>

typedef int64_t (*gs_cb)(int64_t);
#define GS_STACK ((size_t)256 << 10)
static ucontext_t gs_home, gs_co;
static gs_cb gs_fn;
static int64_t gs_arg, gs_out;

static void gs_entry(void) { gs_out = gs_fn(gs_arg); }

static inline int64_t gs_run(gs_cb cb, int64_t n) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    char *map = mmap(NULL, GS_STACK + page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (map == MAP_FAILED) return -2;
    if (mprotect(map, page, PROT_NONE) != 0) return -3;
    gs_fn = cb;
    gs_arg = n;
    if (getcontext(&gs_co) != 0) return -4;
    gs_co.uc_stack.ss_sp = map + page;
    gs_co.uc_stack.ss_size = GS_STACK;
    gs_co.uc_link = &gs_home;
    makecontext(&gs_co, gs_entry, 0);
    if (swapcontext(&gs_home, &gs_co) != 0) return -5;
    return gs_out;
}

typedef struct gs_big {
    int64_t first;
    int64_t rest[4095];
} gs_big;

static int64_t gs_big_read(const gs_big *b) { return b->first; }
static int64_t (*volatile gs_big_reader)(const gs_big *) = gs_big_read;

static inline int64_t gs_big_first(gs_big b) { return gs_big_reader(&b); }

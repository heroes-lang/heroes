/* Defect 2851 (lane sig-family): a coroutine whose context lies on the heap
 * and that yields by a switch the runtime never sees.
 * uh_run(mode, cb): a coroutine on an array of the runner's live frame (the
 *   thread's own stack), entered by the program's `swapcontext`, saves a
 *   buffer and yields, saving into the context it was switched to: on glibc
 *   by the C library's own `swapcontext`, found by walking its dynamic symbol
 *   table by hand, which the runtime never sees; on Darwin, where no door to
 *   the C library's own stands outside the runtime's answers, by the
 *   program's own `swapcontext`, which it sees. A handler then jumps into the
 *   coroutine through the buffer, and the coroutine calls the program: MUST
 *   RUN, the handler saving nothing inside its own frames, so the jump leaves
 *   it (ONSTACK's mirror of defect 2872, lane sig-family-3, step 199r; until
 *   then MUST BE REFUSED, in ONSTACK's words). Mode 0 keeps the coroutine's
 *   context in static memory, 1 in a heap block.
 * rs_case(run, freed, depth, down, cb): a helper runs a coroutine whose
 *   context is a heap block, on a local array of its own frame, to its end
 *   through `uc_link`, and returns; later the same thread saves a point in
 *   frames that occupy that memory, a signal comes `down` frames below it,
 *   and the handler jumps back to the save point; the program is called
 *   after: correct, MUST RUN (`freed` 0: the helper gives the context back
 *   after the coroutine ended; 1, the coroutine gives it back before it ends,
 *   correct on glibc alone, Darwin's `_ctx_done` reading it after, and judged
 *   by no case). */
#if defined(__APPLE__)
#define _XOPEN_SOURCE 700
#define _DARWIN_C_SOURCE 1
#elif !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif
#if !defined(__APPLE__)
#include <link.h>
#endif
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>

typedef int64_t (*uh_cb)(int64_t);
typedef int (*uh_swap_fn)(ucontext_t *, const ucontext_t *);
static ucontext_t uh_main, uh_static;
static ucontext_t *uh_co, *uh_other;
static volatile int uh_mode;
static sigjmp_buf uh_kb, uh_back;
static uh_cb uh_f;
static uh_swap_fn uh_swap;
static volatile int64_t uh_got = -7;

static void uh_body(void) {
    if (sigsetjmp(uh_kb, 0) == 0) uh_swap(uh_mode == 2 ? uh_other : uh_co, &uh_main);
    uh_got = uh_f(4);
    siglongjmp(uh_back, 1);
}

static void uh_handler(int sig) {
    (void)sig;
    siglongjmp(uh_kb, 1);
}

#if !defined(__APPLE__)
/* The C library's own `swapcontext`: its image found by the address of
 * `printf`, its dynamic section walked, the symbols read up to the string
 * table that follows them (glibc's own layout). */
static void *uh_found;
static int uh_image(struct dl_phdr_info *info, size_t size, void *data) {
    (void)size;
    uintptr_t want = (uintptr_t)data, base = (uintptr_t)info->dlpi_addr;
    const ElfW(Dyn) *dyn = NULL;
    int inside = 0;
    for (int i = 0; i < info->dlpi_phnum; i += 1) {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type == PT_DYNAMIC) dyn = (const ElfW(Dyn) *)(base + ph->p_vaddr);
        if (ph->p_type == PT_LOAD && want >= base + ph->p_vaddr && want < base + ph->p_vaddr + ph->p_memsz) inside = 1;
    }
    if (!inside || dyn == NULL) return 0;
    uintptr_t symtab = 0, strtab = 0;
    for (; dyn->d_tag != DT_NULL; dyn += 1) {
        if (dyn->d_tag == DT_SYMTAB) symtab = dyn->d_un.d_ptr;
        if (dyn->d_tag == DT_STRTAB) strtab = dyn->d_un.d_ptr;
    }
    if (symtab < base) symtab += base;
    if (strtab < base) strtab += base;
    if (symtab == 0 || strtab <= symtab) return 1;
    const ElfW(Sym) *sym = (const ElfW(Sym) *)symtab;
    size_t n = (strtab - symtab) / sizeof *sym;
    for (size_t k = 0; k < n; k += 1) {
        if (sym[k].st_value != 0 && ELF64_ST_TYPE(sym[k].st_info) == STT_FUNC &&
            strcmp((const char *)strtab + sym[k].st_name, "swapcontext") == 0) {
            uh_found = (void *)(base + sym[k].st_value);
            return 1;
        }
    }
    return 1;
}

#endif

__attribute__((noinline)) static int64_t uh_run(int32_t heap, uh_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
#if defined(__APPLE__)
    uh_swap = swapcontext;
#else
    dl_iterate_phdr(uh_image, (void *)(uintptr_t)&printf);
    uh_swap = (uh_swap_fn)uh_found;
#endif
    if (uh_swap == NULL) return -9;
    uh_f = cb;
    uh_mode = heap;
            uh_co = heap ? calloc(1, sizeof(ucontext_t)) : &uh_static;
            uh_other = calloc(1, sizeof(ucontext_t));
    if (uh_co == NULL || getcontext(uh_co) != 0) return -1;
    uh_co->uc_stack.ss_sp = local;
    uh_co->uc_stack.ss_size = sizeof local;
    uh_co->uc_link = &uh_main;
    makecontext(uh_co, uh_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = uh_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    swapcontext(&uh_main, uh_co);
    if (sigsetjmp(uh_back, 1) == 0) raise(SIGUSR1);
    __asm__ volatile("" ::"r"(local) : "memory");
    if (heap) free(uh_co);
    return uh_got;
}

static ucontext_t rs_main, *rs_task;
static volatile int rs_free;
static volatile int rs_done;
static void rs_body(void) {
    rs_done = 1;
    if (rs_free) free(rs_task);
}

__attribute__((noinline)) static void rs_helper(int run) {
    char stack[1u << 18];
    stack[0] = 1;
    rs_task = calloc(1, sizeof(ucontext_t));
    getcontext(rs_task);
    rs_task->uc_stack.ss_sp = stack;
    rs_task->uc_stack.ss_size = sizeof stack;
    rs_task->uc_link = &rs_main;
    makecontext(rs_task, rs_body, 0);
    if (run) swapcontext(&rs_main, rs_task);
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
    __asm__ volatile("" ::"r"(pad) : "memory"); /* kept whole at -O2 (step 199p) */
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

static inline int64_t rs_case(int64_t run, int64_t freed, int64_t depth, int64_t down, int64_t (*cb)(int64_t)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = rs_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    rs_free = (int)freed;
    rs_helper((int)run);
    if (!freed) free(rs_task);
    if (run && !rs_done) return -4;
    return rs_deep(depth, down, cb);
}

/* Defect 2868 (lane sig-family-3): a coroutine yielding by a switch the
 * runtime never sees, saving into ANOTHER context than the one it was
 * switched to. The coroutine runs on an array of the runner's live frame (the
 * thread's own stack), its context a heap block, entered by the program's
 * `swapcontext`; it saves a buffer and yields. `how`: 0 by a switch the
 * runtime never sees (glibc's own `swapcontext`, found by walking the C
 * library's dynamic symbol table by hand) saving into its own context; 1 the
 * same saving into another heap context; 2 by the program's own `swapcontext`
 * (seen) into another. On Darwin, where no door to the C library's own stands
 * outside the runtime's answers, every yield is the program's own
 * `swapcontext`, which the runtime sees: the cases hold what both must say.
 * shape 0: the runner raises; the handler, on the thread's own stack, saving
 *   nothing, `siglongjmp`s into the coroutine's buffer; the coroutine calls
 *   the program, then jumps to the runner. The handler is left for good: MUST
 *   RUN (ONSTACK's mirror of defect 2872's `ck`).
 * shape 1: the handler first saves its own point; the coroutine, jumped into,
 *   calls the program while the handler is parked, then jumps back into the
 *   handler, which returns: MUST BE REFUSED.
 * shape 2: the runner resumes the coroutine by the same switch; the coroutine
 *   raises; the handler, on the coroutine's stack, saves its own point and
 *   jumps to the runner's save; the runner calls the program while the
 *   handler is parked, then jumps back into the handler, which returns into
 *   the coroutine, which jumps home: MUST BE REFUSED. */
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

typedef int64_t (*sf_cb)(int64_t);
typedef int (*sf_swap_fn)(ucontext_t *, const ucontext_t *);
static ucontext_t *sf_main, *sf_co, *sf_other;
static volatile int sf_shape, sf_how;
static sigjmp_buf sf_kb, sf_back, sf_back2, sf_hb;
static sf_cb sf_f;
static sf_swap_fn sf_unseen;
static volatile int64_t sf_got = -7;

static void sf_yield(void) {
    if (sf_how == 2) {
        swapcontext(sf_other, sf_main);
    } else {
        sf_unseen(sf_how == 1 ? sf_other : sf_co, sf_main);
    }
}

static void sf_body(void) {
    if (sigsetjmp(sf_kb, 0) == 0) {
        sf_yield();
        if (sf_shape == 2) {
            raise(SIGUSR1);
            siglongjmp(sf_back2, 1);
        }
        return;
    }
    sf_got = sf_f(4);
    if (sf_shape == 1) siglongjmp(sf_hb, 1);
    siglongjmp(sf_back, 1);
}

static void sf_handler(int sig) {
    (void)sig;
    if (sf_shape == 0) siglongjmp(sf_kb, 1);
    if (sigsetjmp(sf_hb, 0) != 0) return;
    if (sf_shape == 1) siglongjmp(sf_kb, 1);
    siglongjmp(sf_back, 1);
}

#if !defined(__APPLE__)
static void *sf_found;
static int sf_image(struct dl_phdr_info *info, size_t size, void *data) {
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
            sf_found = (void *)(base + sym[k].st_value);
            return 1;
        }
    }
    return 1;
}
#endif

__attribute__((noinline)) static int64_t sf_run(int32_t shape, int32_t how, sf_cb cb) {
    _Alignas(16) char local[(size_t)1 << 16];
#if defined(__APPLE__)
    sf_unseen = swapcontext;
#else
    dl_iterate_phdr(sf_image, (void *)(uintptr_t)&printf);
    sf_unseen = (sf_swap_fn)sf_found;
#endif
    if (sf_unseen == NULL) return -9;
    sf_f = cb;
    sf_shape = shape;
    sf_how = how;
    sf_main = calloc(1, sizeof(ucontext_t));
    sf_co = calloc(1, sizeof(ucontext_t));
    sf_other = calloc(1, sizeof(ucontext_t));
    if (sf_main == NULL || sf_co == NULL || sf_other == NULL || getcontext(sf_co) != 0) return -1;
    sf_co->uc_stack.ss_sp = local;
    sf_co->uc_stack.ss_size = sizeof local;
    sf_co->uc_link = sf_main;
    makecontext(sf_co, sf_body, 0);
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sf_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) return -2;
    swapcontext(sf_main, sf_co);
    if (shape == 2) {
        if (sigsetjmp(sf_back2, 1) == 0) {
            if (sigsetjmp(sf_back, 1) == 0) {
                if (how == 2) {
                    swapcontext(sf_main, sf_other);
                } else {
                    sf_unseen(sf_main, how == 1 ? sf_other : sf_co);
                }
                return -5;
            }
            sf_got = sf_f(5);
            siglongjmp(sf_hb, 1);
        }
    } else if (sigsetjmp(sf_back, 1) == 0) {
        raise(SIGUSR1);
    }
    __asm__ volatile("" ::"r"(local) : "memory");
    free(sf_main);
    free(sf_co);
    free(sf_other);
    return sf_got;
}

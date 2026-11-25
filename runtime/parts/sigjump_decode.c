/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_decode.c — route C's key, verified before `main`, and the
 * stack pointer a jump will install, read from the one word of its target
 * (panel 255's R1, defect 2679; design.md §1.12). Needs `sigjump.c` and
 * `sigjump_probe.c`.
 *
 * THE CALIBRATION is a verification and never a search. A probe fills every
 * callee-saved register with a pattern and saves through the C library's own
 * `setjmp`, `_setjmp` and `__sigsetjmp`; `getcontext` and `swapcontext`'s save
 * half are saved the same way. Each save is made at a frame and again 0x9230
 * bytes below it, on the thread's own stack AND on a second stack `mmap`ed for
 * it, and the key is taken only where every word it knows reads as known:
 * the stack pointer's word, a second mangled word under the same key, the
 * patterns where glibc keeps them unmangled. The key and the context field are
 * verified and taken APART: the key is the C library's private, mangled
 * layout, the field its public one, so one failing leaves the other. Where a
 * word does not read as known, or the second stack cannot be mapped, nothing
 * is taken and route A stands for that family (`sigjump_record.c`).
 * `HERO_JMP_FORCE_UNCALIBRATED` (1 the key, 2 the contexts, 3 both) forces
 * the failure, for a build that runs route A where the C library would
 * verify. */

#if !defined(_WIN32) && !defined(__APPLE__)

#if defined(HERO_JMP_DECODES)

static uintptr_t hero_jmp_ror(uintptr_t v, unsigned r) {
    return r == 0 ? v : (v >> r) | (v << (8 * sizeof v - r));
}

/* The stack pointer a jump to `env` will install: one word of it read. */
static uintptr_t hero_jmp_sp_of(const struct __jmp_buf_tag *env) {
    return hero_jmp_ror(((const uintptr_t *)env)[HERO_JMP_SP_WORD], HERO_JMP_ROT) ^
           atomic_load_explicit(&hero_jmp_key, memory_order_relaxed);
}

static uintptr_t hero_jmp_context_sp(const ucontext_t *uc) {
#if defined(__aarch64__)
    return (uintptr_t)uc->uc_mcontext.sp;
#else
    return (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
#endif
}

/* Where that word lies, computed and never read. */
static uintptr_t hero_jmp_context_sp_at(const ucontext_t *uc) {
#if defined(__aarch64__)
    return (uintptr_t)&uc->uc_mcontext.sp;
#else
    return (uintptr_t)&uc->uc_mcontext.gregs[REG_RSP];
#endif
}

/* What the calibration has taken and verified so far, and where a context
 * `swapcontext`'s probe goes to may run. */
typedef struct {
    uintptr_t key;
    bool have;
    bool key_ok;
    bool ctx_ok;
    char *spare;
    size_t spare_size;
} HeroJmpCal;

#define HERO_JMP_SAVE 0
#define HERO_JMP_GET 1
#define HERO_JMP_SWAP 2

/* `swapcontext`'s probe goes to a context that comes straight back into the
 * save half it filled: the C library's own `setcontext` of it. */
static _Atomic(ucontext_t *) hero_jmp_swap_from = NULL;
__attribute__((no_sanitize("address"), noreturn)) static void hero_jmp_swap_return(void) {
    atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(
        atomic_load_explicit(&hero_jmp_swap_from, memory_order_relaxed));
    __builtin_trap();
}

/* One save through the probe, and whether every word the probe knows reads
 * as known: under the key `cal->key` once one is taken (`cal->have`), else
 * under the key the stack pointer's word gives. A context's save (`getcontext`
 * and `swapcontext`'s save half) is verified by its stack-pointer field alone,
 * which is the C library's public layout. */
__attribute__((noinline, no_sanitize("address"))) static bool hero_jmp_try(void *save, int how, HeroJmpCal *cal) {
    union {
        struct __jmp_buf_tag jb[1];
        ucontext_t uc;
    } b;
    ucontext_t to;
    memset(&b, 0, sizeof b);
    HeroJmpProbe p = {0, 0, 0, 0};
    if (how == HERO_JMP_SWAP) {
        memset(&to, 0, sizeof to);
        if (atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed)(&to) != 0) return false;
        to.uc_stack.ss_sp = cal->spare;
        to.uc_stack.ss_size = cal->spare_size;
        to.uc_link = NULL;
        atomic_load_explicit(&hero_ctx_real_make, memory_order_relaxed)(&to, hero_jmp_swap_return, 0);
        atomic_store_explicit(&hero_jmp_swap_from, &b.uc, memory_order_relaxed);
        p.arg = (uintptr_t)&to;
    }
    hero_jmp_probe(&b, save, &p);
    if (how != HERO_JMP_SAVE) return hero_jmp_context_sp(&b.uc) == p.sp;
    const uintptr_t *w = (const uintptr_t *)b.jb;
    uintptr_t k = hero_jmp_ror(w[HERO_JMP_SP_WORD], HERO_JMP_ROT) ^ p.sp;
    if (cal->have && k != cal->key) return false;
    const uintptr_t *pat = hero_jmp_patterns;
#if defined(__aarch64__)
    for (int i = 0; i < 10; i += 1) {
        if (w[i] != pat[i]) return false;
    }
    if (w[10] != p.fp || (w[11] ^ k) != p.ret) return false;
#else
    if (w[0] != pat[0] || (hero_jmp_ror(w[1], 17) ^ k) != pat[1]) return false;
    for (int i = 2; i < 6; i += 1) {
        if (w[i] != pat[i]) return false;
    }
    if ((hero_jmp_ror(w[7], 17) ^ k) != p.ret) return false;
#endif
    cal->key = k;
    cal->have = true;
    return true;
}

/* The same, from a frame 0x9230 bytes further down the stack. */
__attribute__((noinline, no_sanitize("address"))) static bool hero_jmp_try_far(void *save, int how, HeroJmpCal *cal) {
    volatile char *pad = __builtin_alloca(0x9230);
    pad[0] = 0;
    bool ok = hero_jmp_try(save, how, cal);
    __asm__ volatile("" ::"r"(pad) : "memory");
    return ok;
}

/* Every save name the C library has, at a frame and 0x9230 bytes below it,
 * on whatever stack this runs on: the three of the `setjmp` family under one
 * key, then `getcontext` and `swapcontext`'s save half by their field. */
__attribute__((no_sanitize("address"))) static void hero_jmp_calibrate_here(void *data) {
    HeroJmpCal *cal = (HeroJmpCal *)data;
    void *saves[3] = {(void *)atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed),
                      (void *)atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed),
                      (void *)atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed)};
    for (int i = 0; i < 3 && cal->key_ok; i += 1) {
        cal->key_ok = hero_jmp_try(saves[i], HERO_JMP_SAVE, cal) && hero_jmp_try_far(saves[i], HERO_JMP_SAVE, cal);
    }
    void *get = (void *)atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    void *swap = (void *)atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed);
    cal->ctx_ok = cal->ctx_ok && hero_jmp_try(get, HERO_JMP_GET, cal) && hero_jmp_try_far(get, HERO_JMP_GET, cal) &&
                  hero_jmp_try(swap, HERO_JMP_SWAP, cal) && hero_jmp_try_far(swap, HERO_JMP_SWAP, cal);
}

/* The calibration: every callee-saved register a pattern, the
 * stack pointer's word, a second mangled word and the unmangled ones verified
 * under one key, at frames 0x9230 bytes apart on the thread's own stack AND on
 * a second stack `mmap`ed for it (far from the first: every bit of the
 * address moves), for every save name the C library has. A key is taken only
 * where all of it holds; it never searches. Where the second stack cannot be
 * mapped, nothing is taken. */
static void hero_jmp_calibrate(void) {
    size_t half = (size_t)256 << 10;
    char *two = mmap(NULL, 2 * half, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (two == MAP_FAILED) return;
    HeroJmpCal cal = {0, false, true, true, two + half, half};
    hero_jmp_calibrate_here(&cal);
    hero_jmp_on_stack(hero_jmp_calibrate_here, &cal, two + half);
    munmap(two, 2 * half);
#if defined(HERO_JMP_FORCE_UNCALIBRATED)
    if ((HERO_JMP_FORCE_UNCALIBRATED & 1) != 0) cal.key_ok = false;
    if ((HERO_JMP_FORCE_UNCALIBRATED & 2) != 0) cal.ctx_ok = false;
#endif
    if (cal.ctx_ok) atomic_store_explicit(&hero_ctx_decoding, 1, memory_order_release);
    if (!cal.key_ok || !cal.have) return;
    atomic_store_explicit(&hero_jmp_key, cal.key, memory_order_relaxed);
    atomic_store_explicit(&hero_jmp_decoding, 1, memory_order_release);
}
#else
static void hero_jmp_calibrate(void) {}
static uintptr_t hero_jmp_sp_of(const struct __jmp_buf_tag *env) {
    (void)env;
    return 0;
}
static uintptr_t hero_jmp_context_sp(const ucontext_t *uc) {
    (void)uc;
    return 0;
}
static uintptr_t hero_jmp_context_sp_at(const ucontext_t *uc) {
    (void)uc;
    return 0;
}
#endif

/* Before `main`: the C library's own found, then the key verified. A save
 * point reached earlier (a shared library's constructor) finds the C
 * library's own itself and records, since no key is taken yet. */
__attribute__((constructor)) static void hero_jmp_find_early(void) {
    hero_jmp_find();
    hero_jmp_calibrate();
}

#endif

/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_decode.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): the decode and its calibration (rules 1 to 3):
 * the key and the context field, verified before `main`, and the accessor
 * that says which route stands (panel 255's R1, defects 2679 and 2702;
 * design.md §1.12, §4.19). Needs the files `rebind.c` names before it. */

#if defined(__APPLE__)

/* `makecontext`, whose wrapper is assembly (its arguments are variadic). */
typedef void (*hero_rebind_make_fn)(void *, void (*)(void), int, ...);
_Atomic(hero_rebind_make_fn) hero_rebind_real_makecontext = NULL;

/* Darwin arm64's saves (libsystem_platform, read in `dyld_info -disassemble`,
 * panel 255's first round): a `jmp_buf` holds x19..x28 at words 0 to 9,
 * `pacdb(x29, sp) ^ k` at word 10, `x30 ^ k` at word 11 (x30 signed by
 * `pacibsp`), `pacdb(sp, 0xcbed) ^ k` at word 12 and d8..d15 at bytes 0x70 to
 * 0xb0, `k` the thread's pointer-munge word; a context's machine context
 * starts at byte 0x40 and holds x19..x28 from 0xe8, `pacda(x29, 0x4517)` at
 * 0x138, the signed x30 at 0x140, `pacda(sp, 0xcbed)` at 0x148 and d8..d15
 * every 16 bytes from 0x1e8; `setcontext` refuses a context whose size word,
 * 0x28, is 0. `xpacd` and `xpaci` strip a signature where one stands (an
 * arm64e process) and leave the pointer where none does (arm64, measured).
 * Every one of those words is checked before `main`, never assumed. */
#define HERO_REBIND_SP_WORD 12
#define HERO_REBIND_FP_WORD 10
#define HERO_REBIND_LR_WORD 11
#define HERO_REBIND_JMP_D8 0x70
#define HERO_REBIND_CTX_X19 0xe8
#define HERO_REBIND_CTX_FP 0x138
#define HERO_REBIND_CTX_LR 0x140
#define HERO_REBIND_CTX_SP 0x148
#define HERO_REBIND_CTX_D8 0x1e8
#define HERO_REBIND_CTX_SIZE_WORD 0x28

/* Which families decode (rule 3): bit 1 the setjmp family, bit 2 contexts;
 * each set before `main`, once its calibration verified, never after. */
#define HERO_REBIND_JMP 1
#define HERO_REBIND_CTX 2
static _Atomic int hero_rebind_decodes = 0;
static _Atomic uintptr_t hero_rebind_key_k = 0;

/* RULE 3'S ACCESSOR: which path stands, asked by a test (a `run` case through
 * an `extern` header, or the `runtime` suite): bit 1 the setjmp family
 * decodes, bit 2 contexts decode; 0 is route A for both. */
int hero_signal_jump_route(void);
int hero_signal_jump_route(void) { return atomic_load_explicit(&hero_rebind_decodes, memory_order_acquire); }

static bool hero_rebind_decoding(int family) {
    return (atomic_load_explicit(&hero_rebind_decodes, memory_order_acquire) & family) != 0;
}

/* One word of a buffer the C library is about to read. */
static uintptr_t hero_rebind_word(const unsigned char *buf, size_t at) {
    uintptr_t w;
    memcpy(&w, buf + at, sizeof w);
    return w;
}

#if defined(__aarch64__)
static inline uintptr_t hero_rebind_strip_d(uintptr_t p) {
    __asm__("xpacd %0" : "+r"(p));
    return p;
}
static inline uintptr_t hero_rebind_strip_i(uintptr_t p) {
    __asm__("xpaci %0" : "+r"(p));
    return p;
}

/* The stack pointer and the return address a save is handed, stored by the
 * stub that hands them. */
_Atomic uintptr_t hero_rebind_cal_sp = 0;
_Atomic uintptr_t hero_rebind_cal_lr = 0;

/* The patterns the callee-saved registers hold at a calibration's save:
 * x19..x28, x29 (shaped as an address, so a strip leaves it), d8..d15. A
 * register that happens to hold the stack pointer cannot pass for its word. */
static const uint64_t hero_rebind_pattern[19] = {
    UINT64_C(0x1319131913191319), UINT64_C(0x2020202020202020), UINT64_C(0x2121212121212121),
    UINT64_C(0x2222222222222222), UINT64_C(0x2323232323232323), UINT64_C(0x2424242424242424),
    UINT64_C(0x2525252525252525), UINT64_C(0x2626262626262626), UINT64_C(0x2727272727272727),
    UINT64_C(0x2828282828282828), UINT64_C(0x000000F029F02900), UINT64_C(0x0808080808080808),
    UINT64_C(0x0909090909090909), UINT64_C(0x1010101010101010), UINT64_C(0x1111111111111111),
    UINT64_C(0x1212121212121212), UINT64_C(0x1313131313131313), UINT64_C(0x1414141414141414),
    UINT64_C(0x1515151515151515),
};

/* Saves the caller's callee-saved registers, fills them with `pat`, stores
 * the stack pointer and the return address, calls `fn(a, b)`, restores them:
 * `fn` saves the patterns, the stack pointer stored and that return address. */
__attribute__((naked)) static int hero_rebind_cal_call(void *a, void *b, void *fn, const uint64_t *pat) {
    __asm__ volatile("stp x29, x30, [sp, #-16]!\n\t"
                     "mov x29, sp\n\t"
                     "sub sp, sp, #160\n\t"
                     "stp x19, x20, [sp, #0]\n\t"
                     "stp x21, x22, [sp, #16]\n\t"
                     "stp x23, x24, [sp, #32]\n\t"
                     "stp x25, x26, [sp, #48]\n\t"
                     "stp x27, x28, [sp, #64]\n\t"
                     "stp d8, d9, [sp, #80]\n\t"
                     "stp d10, d11, [sp, #96]\n\t"
                     "stp d12, d13, [sp, #112]\n\t"
                     "stp d14, d15, [sp, #128]\n\t"
                     "str x29, [sp, #144]\n\t"
                     "ldp x19, x20, [x3, #0]\n\t"
                     "ldp x21, x22, [x3, #16]\n\t"
                     "ldp x23, x24, [x3, #32]\n\t"
                     "ldp x25, x26, [x3, #48]\n\t"
                     "ldp x27, x28, [x3, #64]\n\t"
                     "ldr x29, [x3, #80]\n\t"
                     "ldp d8, d9, [x3, #88]\n\t"
                     "ldp d10, d11, [x3, #104]\n\t"
                     "ldp d12, d13, [x3, #120]\n\t"
                     "ldp d14, d15, [x3, #136]\n\t"
                     "mov x9, sp\n\t"
                     "adrp x10, _hero_rebind_cal_sp@GOTPAGE\n\t"
                     "ldr x10, [x10, _hero_rebind_cal_sp@GOTPAGEOFF]\n\t"
                     "str x9, [x10]\n\t"
                     "adr x9, 1f\n\t"
                     "adrp x10, _hero_rebind_cal_lr@GOTPAGE\n\t"
                     "ldr x10, [x10, _hero_rebind_cal_lr@GOTPAGEOFF]\n\t"
                     "str x9, [x10]\n\t"
                     "blr x2\n"
                     "1:\n\t"
                     "ldp x19, x20, [sp, #0]\n\t"
                     "ldp x21, x22, [sp, #16]\n\t"
                     "ldp x23, x24, [sp, #32]\n\t"
                     "ldp x25, x26, [sp, #48]\n\t"
                     "ldp x27, x28, [sp, #64]\n\t"
                     "ldp d8, d9, [sp, #80]\n\t"
                     "ldp d10, d11, [sp, #96]\n\t"
                     "ldp d12, d13, [sp, #112]\n\t"
                     "ldp d14, d15, [sp, #128]\n\t"
                     "ldr x29, [sp, #144]\n\t"
                     "add sp, sp, #160\n\t"
                     "ldp x29, x30, [sp], #16\n\t"
                     "ret");
}

/* Runs `fn` on the stack whose top is `top`. */
__attribute__((naked)) static void hero_rebind_on_stack(void *top, void (*fn)(void)) {
    __asm__ volatile("stp x29, x30, [sp, #-16]!\n\t"
                     "mov x29, sp\n\t"
                     "mov x9, sp\n\t"
                     "mov sp, x0\n\t"
                     "stp x9, x10, [sp, #-16]!\n\t"
                     "blr x1\n\t"
                     "ldp x9, x10, [sp], #16\n\t"
                     "mov sp, x9\n\t"
                     "ldp x29, x30, [sp], #16\n\t"
                     "ret");
}

/* Which family failed a check: bit 1 the setjmp family, bit 2 contexts. */
static _Atomic int hero_rebind_cal_bad = 0;

static void hero_rebind_cal_fail(int family) { atomic_fetch_or(&hero_rebind_cal_bad, family); }

/* A `jmp_buf` the save just filled, every word it writes checked. */
static void hero_rebind_cal_jmp(const unsigned char *buf) {
    uintptr_t k = atomic_load(&hero_rebind_key_k);
    for (int i = 0; i < 10; i += 1) {
        if (hero_rebind_word(buf, (size_t)i * 8) != hero_rebind_pattern[i]) hero_rebind_cal_fail(HERO_REBIND_JMP);
    }
    if (hero_rebind_strip_d(hero_rebind_word(buf, HERO_REBIND_FP_WORD * 8) ^ k) != hero_rebind_pattern[10] ||
        hero_rebind_strip_i(hero_rebind_word(buf, HERO_REBIND_LR_WORD * 8) ^ k) != atomic_load(&hero_rebind_cal_lr) ||
        hero_rebind_strip_d(hero_rebind_word(buf, HERO_REBIND_SP_WORD * 8) ^ k) != atomic_load(&hero_rebind_cal_sp)) {
        hero_rebind_cal_fail(HERO_REBIND_JMP);
    }
    for (int i = 0; i < 8; i += 1) {
        if (hero_rebind_word(buf, HERO_REBIND_JMP_D8 + (size_t)i * 8) != hero_rebind_pattern[11 + i]) {
            hero_rebind_cal_fail(HERO_REBIND_JMP);
        }
    }
}

/* A context the save just filled: the registers from `first` (the save half
 * of `swapcontext` keeps x19..x21 for itself), the frame pointer and return
 * address where `exact`, the stack pointer exactly where `exact`, else within
 * the 4 KiB below the caller's. */
static void hero_rebind_cal_ctx(const unsigned char *uc, int first, bool exact) {
    for (int i = first; i < 10; i += 1) {
        if (hero_rebind_word(uc, HERO_REBIND_CTX_X19 + (size_t)i * 8) != hero_rebind_pattern[i]) {
            hero_rebind_cal_fail(HERO_REBIND_CTX);
        }
    }
    for (int i = 0; i < 8; i += 1) {
        if (hero_rebind_word(uc, HERO_REBIND_CTX_D8 + (size_t)i * 16) != hero_rebind_pattern[11 + i]) {
            hero_rebind_cal_fail(HERO_REBIND_CTX);
        }
    }
    uintptr_t sp = hero_rebind_strip_d(hero_rebind_word(uc, HERO_REBIND_CTX_SP));
    uintptr_t want = atomic_load(&hero_rebind_cal_sp);
    if (hero_rebind_word(uc, HERO_REBIND_CTX_SIZE_WORD) == 0) hero_rebind_cal_fail(HERO_REBIND_CTX);
    if (exact) {
        if (sp != want || hero_rebind_strip_d(hero_rebind_word(uc, HERO_REBIND_CTX_FP)) != hero_rebind_pattern[10] ||
            hero_rebind_strip_i(hero_rebind_word(uc, HERO_REBIND_CTX_LR)) != atomic_load(&hero_rebind_cal_lr)) {
            hero_rebind_cal_fail(HERO_REBIND_CTX);
        }
    } else if (sp >= want || sp < want - 4096 || (sp & 15) != 0) {
        hero_rebind_cal_fail(HERO_REBIND_CTX);
    }
}

/* The save half of `swapcontext`: the context it saves is checked on a
 * second stack, then resumed, so the swap returns where it was made. */
static _Atomic(unsigned char *) hero_rebind_cal_from = NULL;
static _Atomic uintptr_t hero_rebind_cal_swap_sp = 0;

__attribute__((no_sanitize("address"))) static void hero_rebind_cal_swapped(void) {
    unsigned char *from = atomic_load(&hero_rebind_cal_from);
    uintptr_t sp = atomic_load(&hero_rebind_cal_sp);
    atomic_store(&hero_rebind_cal_swap_sp, sp);
    hero_rebind_cal_ctx(from, 3, false);
    atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(from);
    /* `setcontext` refuses only a context whose size word is 0, and the save
     * half just wrote it: no way back to the swap remains. */
    __builtin_trap();
}

__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_swap(void) {
    _Alignas(16) unsigned char from[1024], back[1024];
    size_t size = (size_t)64 << 10;
    void *stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    hero_rebind_get_fn get = atomic_load(&hero_rebind_real_getcontext);
    hero_rebind_swap_fn swap = atomic_load(&hero_rebind_real_swapcontext);
    hero_rebind_make_fn make = atomic_load(&hero_rebind_real_makecontext);
    if (stack == MAP_FAILED || get == NULL || swap == NULL || make == NULL) {
        hero_rebind_cal_fail(HERO_REBIND_CTX);
        if (stack != MAP_FAILED) munmap(stack, size);
        return;
    }
    memset(from, 0xA5, sizeof from);
    memset(back, 0, sizeof back);
    if (get(back) != 0) hero_rebind_cal_fail(HERO_REBIND_CTX);
    ucontext_t *uc = (ucontext_t *)(void *)back;
    uc->uc_stack.ss_sp = stack;
    uc->uc_stack.ss_size = size;
    uc->uc_link = NULL;
    make(back, hero_rebind_cal_swapped, 0);
    atomic_store(&hero_rebind_cal_from, from);
    (void)hero_rebind_cal_call(from, back, (void *)swap, hero_rebind_pattern);
    munmap(stack, size);
}

/* Each save the platform has, at this stack pointer. */
__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_check(void) {
    _Alignas(16) unsigned char buf[1024];
    void *saves[4] = {(void *)atomic_load(&hero_rebind_real_setjmp), (void *)atomic_load(&hero_rebind_real__setjmp),
                      (void *)atomic_load(&hero_rebind_real_sigsetjmp), (void *)atomic_load(&hero_rebind_real_sigsetjmp)};
    for (int i = 0; i < 4; i += 1) {
        memset(buf, 0xA5, sizeof buf);
        if (saves[i] == NULL) {
            hero_rebind_cal_fail(HERO_REBIND_JMP);
            continue;
        }
        (void)hero_rebind_cal_call(buf, (void *)(uintptr_t)(i == 3), saves[i], hero_rebind_pattern);
        hero_rebind_cal_jmp(buf);
    }
    void *get = (void *)atomic_load(&hero_rebind_real_getcontext);
    if (get == NULL) {
        hero_rebind_cal_fail(HERO_REBIND_CTX);
    } else {
        memset(buf, 0xA5, sizeof buf);
        (void)hero_rebind_cal_call(buf, NULL, get, hero_rebind_pattern);
        hero_rebind_cal_ctx(buf, 0, true);
    }
    hero_rebind_cal_swap();
}

__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_deep(int n) {
    volatile unsigned char pad[64 * 1024];
    pad[0] = (unsigned char)n;
    if (n > 0) {
        hero_rebind_cal_deep(n - 1);
    } else {
        hero_rebind_cal_check();
    }
    (void)pad[0];
}

/* `k` from one `_setjmp` at a known stack pointer, then every save checked at
 * this frame, 1 MiB deeper and on a second, `mmap`ed stack; a family that
 * fails any check, or that the build forces to fail, stays on route A. */
__attribute__((no_sanitize("address"))) static void hero_rebind_calibrate(void) {
    _Alignas(16) unsigned char buf[256];
    void *save = (void *)atomic_load(&hero_rebind_real__setjmp);
    if (save == NULL) return;
    memset(buf, 0xA5, sizeof buf);
    (void)hero_rebind_cal_call(buf, NULL, save, hero_rebind_pattern);
    atomic_store(&hero_rebind_key_k, hero_rebind_word(buf, HERO_REBIND_SP_WORD * 8) ^ atomic_load(&hero_rebind_cal_sp));
    hero_rebind_cal_check();
    hero_rebind_cal_deep(16);
    size_t size = (size_t)1 << 18;
    void *stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (stack == MAP_FAILED) return;
    hero_rebind_on_stack((unsigned char *)stack + size, hero_rebind_cal_check);
    munmap(stack, size);
    int bad = atomic_load(&hero_rebind_cal_bad);
#if defined(HERO_JMP_FORCE_UNCALIBRATED)
    bad |= HERO_JMP_FORCE_UNCALIBRATED;
#endif
    atomic_store_explicit(&hero_rebind_decodes, (HERO_REBIND_JMP | HERO_REBIND_CTX) & ~bad, memory_order_release);
}
#else
/* Darwin x86-64: no decode is read here (no machine to calibrate it on), so
 * both families stay on route A, by construction. */
static inline uintptr_t hero_rebind_strip_d(uintptr_t p) { return p; }
static void hero_rebind_calibrate(void) {}
#endif

#endif

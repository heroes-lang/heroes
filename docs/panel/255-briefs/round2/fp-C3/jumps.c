/* -- route C: the target's stack pointer, decoded ------------------------ */

/* Darwin arm64's `_setjmp` (libsystem_platform, read in `dyld_info
 * -disassemble`): word 12 is `pacdb(sp, 0xcbed) ^ k`, `k` the process's
 * pointer-munge token; a context's stack pointer is its machine context's
 * `__ss.__sp`, `pacda(sp, 0xcbed)`, at byte 0x148 where `setcontext` reads it.
 * `xpacd` strips a signature where one stands (an arm64e process) and leaves
 * the pointer where none does (arm64, measured: the data keys sign nothing
 * there). `k` is taken from one save at a known stack pointer and verified
 * before `main` at two more stack pointers 1 MiB apart and on a second stack,
 * for each of the four saves and `getcontext`; where any check fails nothing
 * is decoded and every jump inside a handler keeps every entry, told. */
#define HERO_REBIND_SP_WORD 12
#define HERO_REBIND_CTX_SP 0x148
static uintptr_t hero_rebind_key_k = 0;
static _Atomic int hero_rebind_decodes = 0;

#if defined(__aarch64__)
static inline uintptr_t hero_rebind_strip(uintptr_t p) {
    __asm__("xpacd %0" : "+r"(p));
    return p;
}

/* The stack pointer a save is handed, stored by the stub that hands it. */
uintptr_t hero_rebind_cal_sp = 0;

/* Stores the stack pointer, then branches to `fn` with the caller's frame
 * untouched, so `fn` saves the stack pointer stored. */
__attribute__((naked)) static int hero_rebind_cal_call(void *buf, int mask, void *fn) {
    __asm__ volatile("mov x9, sp\n\t"
                     "adrp x10, _hero_rebind_cal_sp@GOTPAGE\n\t"
                     "ldr x10, [x10, _hero_rebind_cal_sp@GOTPAGEOFF]\n\t"
                     "str x9, [x10]\n\t"
                     "br x2");
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

static _Atomic int hero_rebind_cal_bad = 0;

/* Each of the four saves and `getcontext` at this stack pointer, decoded. */
__attribute__((noinline, no_sanitize("address"))) static void hero_rebind_cal_check(void) {
    _Alignas(16) unsigned char buf[1024];
    void *saves[4] = {(void *)atomic_load(&hero_rebind_real_setjmp), (void *)atomic_load(&hero_rebind_real__setjmp),
                      (void *)atomic_load(&hero_rebind_real_sigsetjmp), (void *)atomic_load(&hero_rebind_real_sigsetjmp)};
    for (int i = 0; i < 4; i += 1) {
        uintptr_t word;
        memset(buf, 0xA5, sizeof buf);
        if (saves[i] == NULL) {
            atomic_store(&hero_rebind_cal_bad, 1);
            continue;
        }
        (void)hero_rebind_cal_call(buf, i == 3, saves[i]);
        memcpy(&word, buf + HERO_REBIND_SP_WORD * sizeof(uintptr_t), sizeof word);
        if (hero_rebind_strip(word ^ hero_rebind_key_k) != hero_rebind_cal_sp) atomic_store(&hero_rebind_cal_bad, 1);
    }
    void *get = (void *)atomic_load(&hero_rebind_real_getcontext);
    if (get != NULL) {
        uintptr_t word;
        memset(buf, 0xA5, sizeof buf);
        (void)hero_rebind_cal_call(buf, 0, get);
        memcpy(&word, buf + HERO_REBIND_CTX_SP, sizeof word);
        if (hero_rebind_strip(word) != hero_rebind_cal_sp) atomic_store(&hero_rebind_cal_bad, 1);
    }
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

__attribute__((no_sanitize("address"))) static void hero_rebind_calibrate(void) {
    _Alignas(16) unsigned char buf[256];
    void *save = (void *)atomic_load(&hero_rebind_real__setjmp);
    if (save == NULL) return;
    memset(buf, 0xA5, sizeof buf);
    (void)hero_rebind_cal_call(buf, 0, save);
    uintptr_t word;
    memcpy(&word, buf + HERO_REBIND_SP_WORD * sizeof(uintptr_t), sizeof word);
    hero_rebind_key_k = word ^ hero_rebind_cal_sp;
    hero_rebind_cal_check();
    hero_rebind_cal_deep(16);
    size_t size = (size_t)1 << 18;
    void *stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (stack == MAP_FAILED) return;
    hero_rebind_on_stack((unsigned char *)stack + size, hero_rebind_cal_check);
    munmap(stack, size);
    if (!atomic_load(&hero_rebind_cal_bad)) atomic_store_explicit(&hero_rebind_decodes, 1, memory_order_release);
}
#else
static inline uintptr_t hero_rebind_strip(uintptr_t p) { return p; }
static void hero_rebind_calibrate(void) {}
#endif

/* A jump whose target's stack pointer is `sp`: outside every handler nothing
 * read; else the entries it proves left dropped; where nothing decodes, every
 * entry kept and marked as one this jump may have left. */
static void hero_rebind_leave(const void *target, size_t at, uintptr_t key) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || own->depth == 0) return;
    if (!atomic_load_explicit(&hero_rebind_decodes, memory_order_acquire)) {
        own->unsure = own->depth;
        return;
    }
    uintptr_t word;
    memcpy(&word, (const unsigned char *)target + at, sizeof word);
    hero_rebind_prune(own, hero_rebind_strip(word ^ key));
}

static void hero_rebind_jump(const void *env) {
    hero_rebind_leave(env, HERO_REBIND_SP_WORD * sizeof(uintptr_t), hero_rebind_key_k);
}

static void hero_rebind_jump_context(const void *context) { hero_rebind_leave(context, HERO_REBIND_CTX_SP, 0); }

/* -- the jumps ------------------------------------------------------------ */

static int hero_rebind_swapcontext(void *from, const void *to) {
    hero_rebind_jump_context(to);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_swapcontext, memory_order_relaxed)(from,
                                                                                                                to);
}

__attribute__((noreturn)) static void hero_rebind_siglongjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real_siglongjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn)) static void hero_rebind_longjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real_longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn)) static void hero_rebind__longjmp(int *env, int value) {
    hero_rebind_jump(env);
    atomic_load_explicit(&hero_rebind_real__longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

static int hero_rebind_setcontext(const void *context) {
    hero_rebind_jump_context(context);
    return atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
}


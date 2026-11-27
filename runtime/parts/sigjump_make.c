/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_make.c — `makecontext`, which names the stack a context will
 * run on and so makes it a known one of this thread's process (rule 7, panel
 * 255's R3, defect 2702; design.md §1.12). Needs `sigjump_save.c`.
 *
 * The C library reads `uc_stack` here and nowhere else, so the range noted is
 * the one the context will run on. Where the context's own field does not
 * decode (route A for contexts) its record goes, the context now naming
 * another stack, and its range is held (defect 2745).
 *
 * ON AARCH64 AND X86-64 THE WRAPPER IS ASSEMBLY: `makecontext`'s arguments are
 * variadic, which `musttail` refuses, so the argument registers are kept, the
 * range noted, then a branch to the C library's own with the stack as the
 * caller left it (Darwin's `parts/rebind_jump.c` does the same), every
 * argument passed on whatever their count.
 *
 * ON GLIBC'S OTHER ARCHITECTURES THE WRAPPER IS C (defect d-lane-ctx-after-1):
 * until then no wrapper stood there, no stack was known, and a coroutine
 * started by `setcontext`, or a handler's `setcontext` into a context
 * `makecontext` made, ran the program's function inside the signal; a save
 * made inside a `swapcontext` the thread made stood in for the range (step
 * 187b's placement, now gone), which no `setcontext` reached. C cannot pass a
 * variadic list on, so the wrapper reads up to `HERO_JMP_MAKE_ARGS` of them,
 * each as a `long`, the width of one argument's slot on each of glibc's
 * architectures and the type glibc's own `makecontext` reads them as on the
 * 64-bit ones, and calls the C library's own with that many: one that reads
 * `argc` of them reads the program's own bits. A `makecontext` given more is
 * stopped, saying so, before anything is made: passing a part of them on
 * would start the coroutine with words the program never gave. It is the
 * one wrapper `HERO_JMP_MAKE_IN_C` builds on aarch64 too, so the path glibc's
 * other architectures take is run there (`sigjump.c`).
 *
 * THE END OF A COROUTINE IS SEEN WHERE ITS FUNCTION RETURNS (defect 2868).
 * glibc's start routine runs once the function has returned and switches to
 * the context's `uc_link` or ends the thread, both unseen. Where contexts
 * decode (aarch64 and x86-64), at the first switch this file sees into a
 * context `makecontext` made and nothing has run (`hero_ctx_watch_end`), the
 * function's end is made to pass through this file, in the layout the C
 * library's own `makecontext` was read to write before `main`
 * (`sigjump_decode.c`): where the function returns into the routine (the
 * layout read on x86-64), its return names `hero_ctx_end`; where the routine
 * calls the function from x20 (glibc 2.41 on aarch64, measured), x20 names
 * `hero_ctx_call`, which calls the function, moved to x21, as the routine
 * would. Either then tells the range its coroutine ended, makes the same
 * change to the context the C library switches to next and enters the range
 * that one lands on, and goes on into the C library's routine with every
 * register it reads as it found it. The context is written only at a switch into it, which the C library
 * reads next, so it lives; never by `makecontext`'s wrapper, which reaches the
 * C library's own by a branch and so never runs after it. Until then the end
 * was judged by what the switches made from the range and the context
 * switched to did not show, and a coroutine yielding by a switch the runtime
 * never sees, saving into another context, was judged ended while alive. */

#if !defined(_WIN32) && !defined(__APPLE__)

__attribute__((used, visibility("hidden"))) void hero_sig_made_context(const void *context);
__attribute__((used, visibility("hidden"))) void hero_sig_made_context(const void *context) {
    hero_jmp_find();
    const ucontext_t *uc = (const ucontext_t *)context;
    HeroSignalStack *own = hero_sig_own();
    if (own != NULL && uc != NULL) {
        uintptr_t lo = (uintptr_t)uc->uc_stack.ss_sp;
        hero_signal_switch_remade(own, lo, lo + (uintptr_t)uc->uc_stack.ss_size);
    }
    /* Route A reads no switch's target, so it cannot see the range entered
     * or its coroutine end: the range is held from here on, and a target in
     * it lies on the coroutine (rule 7, defect 2745). */
    hero_sig_known_stack(context, hero_ctx_recording());
    if (hero_ctx_recording()) hero_jmp_forget(context);
}

/* The return of a context's function sent through `hero_ctx_end`. Called at
 * a switch into `ucp`, which the C library reads next; where `ucp` is a
 * context `makecontext` made that nothing has run (its return still the C
 * library's start routine) on a range `makecontext` was handed, its return
 * is rewritten, on aarch64 in the context's link-register slot and on x86-64
 * in the word its stack pointer names, on that range. Loads and stores of
 * the context alone: async-signal-safe. */
static void hero_ctx_watch_end(const ucontext_t *ucp);

/* `hero_ctx_end`'s own work, on the stack of the coroutine whose function
 * returned (`sp`, its stack pointer then), before the C library switches to
 * `link` (its `uc_link`, none for a thread's end). */
__attribute__((used, visibility("hidden"))) void hero_ctx_ended(uintptr_t sp, const ucontext_t *link);

#if defined(HERO_JMP_DECODES) && defined(__aarch64__)
/* Entered by the function's `ret`, with glibc's `uc_link` in x19, which the
 * start routine reads; x19 survives the call, as it survives the function. */
__asm__(".text\n.p2align 2\n.globl hero_ctx_end\n.hidden hero_ctx_end\n.type hero_ctx_end, %function\n"
        "hero_ctx_end:\n"
        "  mov x0, sp\n"
        "  mov x1, x19\n"
        "  bl hero_ctx_ended\n"
        "  adrp x16, hero_ctx_start_routine\n"
        "  ldr x16, [x16, :lo12:hero_ctx_start_routine]\n"
        "  br x16\n"
        ".size hero_ctx_end, .-hero_ctx_end\n");
/* Called by glibc's start routine (`blr x20`) in place of the function, which
 * is in x21, with the arguments in x0 to x7 and on the stack as the routine
 * left them, and `uc_link` in x19: the function called with all of them, then
 * the end told, then a return into the routine, x19 kept. x21 and x22 survive
 * both calls. */
__asm__(".text\n.p2align 2\n.globl hero_ctx_call\n.hidden hero_ctx_call\n.type hero_ctx_call, %function\n"
        "hero_ctx_call:\n"
        "  hint #34\n"
        "  mov x22, x30\n"
        "  blr x21\n"
        "  mov x0, sp\n"
        "  mov x1, x19\n"
        "  bl hero_ctx_ended\n"
        "  mov x30, x22\n"
        "  ret\n"
        ".size hero_ctx_call, .-hero_ctx_call\n");
#elif defined(HERO_JMP_DECODES) && defined(__x86_64__)
/* Entered by the function's `ret`, `rsp` 16-aligned, with glibc's `rbx`
 * naming the word that holds `uc_link`, which the start routine reads; `rbx`
 * survives the call, as it survives the function. */
__asm__(".text\n.globl hero_ctx_end\n.hidden hero_ctx_end\n.type hero_ctx_end, @function\n"
        "hero_ctx_end:\n"
        "  mov %rsp, %rdi\n"
        "  mov (%rbx), %rsi\n"
        "  call hero_ctx_ended\n"
        "  jmp *hero_ctx_start_routine(%rip)\n"
        ".size hero_ctx_end, .-hero_ctx_end\n");
#endif

#if defined(HERO_JMP_DECODES)
void hero_ctx_end(void);
#if defined(__aarch64__)
void hero_ctx_call(void);
#endif

static void hero_ctx_watch_end(const ucontext_t *ucp) {
    uintptr_t start = atomic_load_explicit(&hero_ctx_start_routine, memory_order_acquire);
    uintptr_t lo = 0, hi = 0;
    if (start == 0 || ucp == NULL || !hero_sig_known_range(hero_jmp_context_sp(ucp), &lo, &hi)) return;
    ucontext_t *uc = (ucontext_t *)(uintptr_t)ucp;
#if defined(__aarch64__)
    if (atomic_load_explicit(&hero_ctx_start_layout, memory_order_relaxed) == HERO_CTX_CALLS_FROM) {
        if ((uintptr_t)uc->uc_mcontext.pc != start || uc->uc_mcontext.regs[20] == (uintptr_t)hero_ctx_call) return;
        uc->uc_mcontext.regs[21] = uc->uc_mcontext.regs[20];
        uc->uc_mcontext.regs[20] = (uintptr_t)hero_ctx_call;
    } else if ((uintptr_t)uc->uc_mcontext.regs[30] == start) {
        uc->uc_mcontext.regs[30] = (uintptr_t)hero_ctx_end;
    }
#else
    uintptr_t sp = (uintptr_t)uc->uc_mcontext.gregs[REG_RSP];
    if (sp < lo || sp > hi - sizeof(uintptr_t)) return;
    uintptr_t *ret = (uintptr_t *)sp;
    if (*ret == start) *ret = (uintptr_t)hero_ctx_end;
#endif
}

void hero_ctx_ended(uintptr_t sp, const ucontext_t *link) {
    hero_sig_range_ended(sp);
    if (link == NULL) return;
    hero_ctx_watch_end(link);
    hero_sig_range_entered(hero_jmp_context_sp(link));
}
#else
static void hero_ctx_watch_end(const ucontext_t *ucp) { (void)ucp; }
void hero_ctx_ended(uintptr_t sp, const ucontext_t *link) {
    (void)sp;
    (void)link;
}
#endif

#if defined(HERO_JMP_MAKE_ASM) && defined(__aarch64__)
__asm__(".text\n.p2align 2\n.globl makecontext\n.type makecontext, %function\n"
        "makecontext:\n"
        "  stp x29, x30, [sp, #-80]!\n"
        "  mov x29, sp\n"
        "  stp x0, x1, [sp, #16]\n"
        "  stp x2, x3, [sp, #32]\n"
        "  stp x4, x5, [sp, #48]\n"
        "  stp x6, x7, [sp, #64]\n"
        "  bl hero_sig_made_context\n"
        "  ldp x0, x1, [sp, #16]\n"
        "  ldp x2, x3, [sp, #32]\n"
        "  ldp x4, x5, [sp, #48]\n"
        "  ldp x6, x7, [sp, #64]\n"
        "  ldp x29, x30, [sp], #80\n"
        "  adrp x16, hero_ctx_real_make\n"
        "  ldr x16, [x16, :lo12:hero_ctx_real_make]\n"
        "  br x16\n"
        ".size makecontext, .-makecontext\n");
#elif defined(HERO_JMP_MAKE_ASM) && defined(__x86_64__)
__asm__(".text\n.globl makecontext\n.type makecontext, @function\n"
        "makecontext:\n"
        "  endbr64\n"
        "  push %rdi\n"
        "  push %rsi\n"
        "  push %rdx\n"
        "  push %rcx\n"
        "  push %r8\n"
        "  push %r9\n"
        "  push %rax\n"
        "  call hero_sig_made_context\n"
        "  pop %rax\n"
        "  pop %r9\n"
        "  pop %r8\n"
        "  pop %rcx\n"
        "  pop %rdx\n"
        "  pop %rsi\n"
        "  pop %rdi\n"
        "  mov hero_ctx_real_make(%rip), %r11\n"
        "  jmp *%r11\n"
        ".size makecontext, .-makecontext\n");
#else
#include <stdarg.h>

/* The count of arguments the C wrapper passes on, read in eights. */
#define HERO_JMP_MAKE_ARGS 64
#define HERO_JMP_MAKE_8(a, k) a[k], a[k + 1], a[k + 2], a[k + 3], a[k + 4], a[k + 5], a[k + 6], a[k + 7]

/* `makecontext` is no signal-safe call, so the stop is the runtime's ordinary
 * panic. */
static _Noreturn void hero_jmp_make_refuse(int argc) {
    char said[320];
    snprintf(said, sizeof said,
             "`makecontext` was given %d arguments for its function, and on this machine this runtime passes at "
             "most %d of them on to the C library's own `makecontext`, which would start the coroutine with words "
             "the program never gave: give the function at most %d arguments, or one pointer to a record holding "
             "them",
             argc, HERO_JMP_MAKE_ARGS, HERO_JMP_MAKE_ARGS);
    hero_panic(said);
}

void(makecontext)(ucontext_t *ucp, void (*func)(void), int argc, ...) {
    if (argc > HERO_JMP_MAKE_ARGS) hero_jmp_make_refuse(argc);
    long a[HERO_JMP_MAKE_ARGS];
    memset(a, 0, sizeof a);
    va_list ap;
    va_start(ap, argc);
    for (int i = 0; i < argc; i += 1) a[i] = va_arg(ap, long);
    va_end(ap);
    hero_sig_made_context(ucp);
    atomic_load_explicit(&hero_ctx_real_make, memory_order_relaxed)(
        ucp, func, argc, HERO_JMP_MAKE_8(a, 0), HERO_JMP_MAKE_8(a, 8), HERO_JMP_MAKE_8(a, 16), HERO_JMP_MAKE_8(a, 24),
        HERO_JMP_MAKE_8(a, 32), HERO_JMP_MAKE_8(a, 40), HERO_JMP_MAKE_8(a, 48), HERO_JMP_MAKE_8(a, 56));
}
#endif

#endif

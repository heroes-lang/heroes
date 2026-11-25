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
 * the one the context will run on. `makecontext`'s arguments are variadic,
 * which `musttail` refuses, so the wrapper is assembly: the argument registers
 * kept, the range noted, then a branch to the C library's own with the stack
 * as the caller left it (Darwin's `parts/rebind_jump.c` does the same). Where
 * the context's own field does not decode (route A for contexts) its record
 * goes, the context now naming another stack, and its range is held
 * (defect 2745). On glibc's other architectures
 * no wrapper is defined: no context stack is known there, and a coroutine's
 * frames are of unknown extent, which proves no handler left (a told refusal). */

#if !defined(_WIN32) && !defined(__APPLE__) && (defined(__aarch64__) || defined(__x86_64__))

__attribute__((used, visibility("hidden"))) void hero_sig_made_context(const void *context);
__attribute__((used, visibility("hidden"))) void hero_sig_made_context(const void *context) {
    hero_jmp_find();
    /* Route A reads no switch's target, so it cannot see the range entered
     * or its coroutine end: the range is held from here on, and a target in
     * it lies on the coroutine (rule 7, defect 2745). */
    hero_sig_known_stack(context, hero_ctx_recording());
    if (hero_ctx_recording()) hero_jmp_forget(context);
}

#if defined(__aarch64__)
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
#elif defined(__x86_64__)
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
#endif

#endif

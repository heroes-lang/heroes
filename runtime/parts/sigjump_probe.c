/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_probe.c — the assembly route C's calibration saves through
 * (panel 255's R1; design.md §1.12). Two stubs per architecture, written in
 * assembly because the registers they fill are the C library's to save and a
 * C function would not leave them as set:
 *
 *   `hero_jmp_probe`     fills every callee-saved register with a known
 *                        pattern, then calls the C library's save with its
 *                        own return address and stack pointer noted, so the
 *                        words the save writes can be told one from another;
 *   `hero_jmp_on_stack`  calls a function on a second stack and returns, so
 *                        the same saves are verified far from the thread's.
 *
 * Only glibc's two architectures have them (`HERO_JMP_DECODES`); anywhere
 * else route A stands. Also here, for the decode to read: where the stack
 * pointer lies in a `jmp_buf` and how it is rotated, each measured on glibc
 * 2.41 and 2.43 (panel 255's Q2). Needs `sigjump.c`. */

#if !defined(_WIN32) && !defined(__APPLE__) && defined(HERO_JMP_DECODES)

/* What the probe saw: the stack pointer the C library's save was called at,
 * the address it returns to, and (arm64) the probe's frame pointer. */
typedef struct {
    uintptr_t sp;
    uintptr_t ret;
    uintptr_t fp;
    uintptr_t arg;
} HeroJmpProbe;

void hero_jmp_probe(void *buf, void *save, HeroJmpProbe *out);
extern const uintptr_t hero_jmp_patterns[];

#if defined(__aarch64__)
/* glibc's aarch64 `jmpbuf-offsets.h`: x19 to x28 in words 0 to 9 and x29 in
 * 10 as they are, the link register in 11 and the stack pointer in 13 each
 * `^` the pointer guard. Measured, not assumed (panel 255's Q2). */
#define HERO_JMP_SP_WORD 13
#define HERO_JMP_ROT 0
__asm__(".section .rodata\n.p2align 3\n.globl hero_jmp_patterns\n.hidden hero_jmp_patterns\n"
        "hero_jmp_patterns:\n"
        "  .quad 0x4845524f45530013, 0x4845524f45530014, 0x4845524f45530015, 0x4845524f45530016\n"
        "  .quad 0x4845524f45530017, 0x4845524f45530018, 0x4845524f45530019, 0x4845524f4553001a\n"
        "  .quad 0x4845524f4553001b, 0x4845524f4553001c\n"
        ".text\n.p2align 2\n.globl hero_jmp_probe\n.hidden hero_jmp_probe\n.type hero_jmp_probe, %function\n"
        "hero_jmp_probe:\n"
        "  stp x29, x30, [sp, #-96]!\n"
        "  mov x29, sp\n"
        "  stp x19, x20, [sp, #16]\n"
        "  stp x21, x22, [sp, #32]\n"
        "  stp x23, x24, [sp, #48]\n"
        "  stp x25, x26, [sp, #64]\n"
        "  stp x27, x28, [sp, #80]\n"
        "  sub sp, sp, #32\n"
        "  adrp x9, hero_jmp_patterns\n"
        "  add x9, x9, :lo12:hero_jmp_patterns\n"
        "  ldp x19, x20, [x9, #0]\n"
        "  ldp x21, x22, [x9, #16]\n"
        "  ldp x23, x24, [x9, #32]\n"
        "  ldp x25, x26, [x9, #48]\n"
        "  ldp x27, x28, [x9, #64]\n"
        "  mov x9, sp\n"
        "  str x9, [x2, #0]\n"
        "  adr x9, 1f\n"
        "  str x9, [x2, #8]\n"
        "  str x29, [x2, #16]\n"
        "  mov x9, x1\n"
        "  ldr x1, [x2, #24]\n"
        "  blr x9\n"
        "1:\n"
        "  add sp, sp, #32\n"
        "  ldp x19, x20, [sp, #16]\n"
        "  ldp x21, x22, [sp, #32]\n"
        "  ldp x23, x24, [sp, #48]\n"
        "  ldp x25, x26, [sp, #64]\n"
        "  ldp x27, x28, [sp, #80]\n"
        "  ldp x29, x30, [sp], #96\n"
        "  ret\n"
        ".size hero_jmp_probe, .-hero_jmp_probe\n");
#else
/* glibc's x86-64 `jmpbuf-offsets.h`: rbx, r12 to r15 in words 0, 2 to 5 as
 * they are; rbp in 1, the stack pointer in 6 and the return address in 7
 * each `^` the pointer guard and then rotated left by 17. Measured, not
 * assumed (panel 255's Q2). */
#define HERO_JMP_SP_WORD 6
#define HERO_JMP_ROT 17
__asm__(".section .rodata\n.p2align 3\n.globl hero_jmp_patterns\n.hidden hero_jmp_patterns\n"
        "hero_jmp_patterns:\n"
        "  .quad 0x4845524f45530003, 0x4845524f45530005, 0x4845524f4553000c, 0x4845524f4553000d\n"
        "  .quad 0x4845524f4553000e, 0x4845524f4553000f\n"
        ".text\n.globl hero_jmp_probe\n.hidden hero_jmp_probe\n.type hero_jmp_probe, @function\n"
        "hero_jmp_probe:\n"
        "  push %rbp\n"
        "  push %rbx\n"
        "  push %r12\n"
        "  push %r13\n"
        "  push %r14\n"
        "  push %r15\n"
        "  sub $8, %rsp\n"
        "  lea hero_jmp_patterns(%rip), %rax\n"
        "  mov 0(%rax), %rbx\n"
        "  mov 8(%rax), %rbp\n"
        "  mov 16(%rax), %r12\n"
        "  mov 24(%rax), %r13\n"
        "  mov 32(%rax), %r14\n"
        "  mov 40(%rax), %r15\n"
        "  mov %rsp, 0(%rdx)\n"
        "  lea 1f(%rip), %rax\n"
        "  mov %rax, 8(%rdx)\n"
        "  mov %rsi, %r11\n"
        "  mov 24(%rdx), %rsi\n"
        "  call *%r11\n"
        "1:\n"
        "  add $8, %rsp\n"
        "  pop %r15\n"
        "  pop %r14\n"
        "  pop %r13\n"
        "  pop %r12\n"
        "  pop %rbx\n"
        "  pop %rbp\n"
        "  ret\n"
        ".size hero_jmp_probe, .-hero_jmp_probe\n");
#endif

/* A call of `fn(arg)` on another stack whose top is `top` (16-aligned), and
 * back: the calibration's second stack. */
void hero_jmp_on_stack(void (*fn)(void *), void *arg, void *top);
#if defined(__aarch64__)
__asm__(".text\n.p2align 2\n.globl hero_jmp_on_stack\n.hidden hero_jmp_on_stack\n.type hero_jmp_on_stack, %function\n"
        "hero_jmp_on_stack:\n"
        "  stp x29, x30, [sp, #-16]!\n"
        "  mov x29, sp\n"
        "  mov sp, x2\n"
        "  mov x9, x0\n"
        "  mov x0, x1\n"
        "  blr x9\n"
        "  mov sp, x29\n"
        "  ldp x29, x30, [sp], #16\n"
        "  ret\n"
        ".size hero_jmp_on_stack, .-hero_jmp_on_stack\n");
#else
__asm__(".text\n.globl hero_jmp_on_stack\n.hidden hero_jmp_on_stack\n.type hero_jmp_on_stack, @function\n"
        "hero_jmp_on_stack:\n"
        "  push %rbp\n"
        "  mov %rsp, %rbp\n"
        "  mov %rdx, %rsp\n"
        "  mov %rdi, %rax\n"
        "  mov %rsi, %rdi\n"
        "  call *%rax\n"
        "  mov %rbp, %rsp\n"
        "  pop %rbp\n"
        "  ret\n"
        ".size hero_jmp_on_stack, .-hero_jmp_on_stack\n");
#endif

#endif

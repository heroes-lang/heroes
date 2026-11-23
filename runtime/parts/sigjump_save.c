/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_save.c — the C library's save points and jumps, defined here
 * so a program's own calls reach them (panel 255's R1, defect 2679; defect
 * 2522; design.md §1.12). The save points are `setjmp`, `_setjmp`,
 * `__sigsetjmp` (glibc's `sigsetjmp` is a macro for it) and `getcontext`; the
 * jumps `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk` (the fortified
 * one) and `setcontext`; and `swapcontext`, which saves and jumps. A save
 * point records only where route A stands, then reaches the C library's own
 * by a branch; a jump drops the entries its target leaves
 * (`sigjump_record.c`'s `hero_jmp_leave`) and then jumps. Needs
 * `sigjump_record.c`. */

#if !defined(_WIN32) && !defined(__APPLE__)

/* Whether the save points must record: only where the key is not verified. */
static bool hero_jmp_recording(void) { return atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) == 0; }
static bool hero_ctx_recording(void) { return atomic_load_explicit(&hero_ctx_decoding, memory_order_acquire) == 0; }

static _Noreturn void hero_jmp_go(hero_jmp_jump_fn real, struct __jmp_buf_tag *env, int val) {
#if defined(HERO_JMP_ASAN)
    __asan_handle_no_return();
#endif
    real(env, val);
    __builtin_unreachable();
}

/* THE SAVE POINTS: under A the frame noted, then the C library's own reached
 * by a branch; under C the branch alone. The names are parenthesised so
 * glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uintptr_t)__builtin_frame_address(0), (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    if (hero_ctx_recording()) hero_jmp_note(ucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_ctx_get_fn real = atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    __attribute__((musttail)) return real(ucp);
}

/* THE JUMPS. Each finds the C library's own first, as a save point does. */
void(longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_longjmp, memory_order_relaxed), env, val);
}

void(_longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real__longjmp, memory_order_relaxed), env, val);
}

void(siglongjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_siglongjmp, memory_order_relaxed), env, val);
}

void __longjmp_chk(struct __jmp_buf_tag env[1], int val);
void __longjmp_chk(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_leave(env, false);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_chk, memory_order_relaxed), env, val);
}

int(setcontext)(const ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_leave(ucp, true);
    return atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
}

/* Saves into `oucp` from this wrapper's own frame, which stays live while the
 * other context runs and returns through it, then leaves for `ucp`. */
int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    hero_jmp_find();
    if (hero_ctx_recording()) hero_jmp_note(oucp, (uintptr_t)__builtin_frame_address(0), HERO_JMP_CONTEXT_BYTES);
    hero_jmp_leave(ucp, true);
    return atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
}

#endif

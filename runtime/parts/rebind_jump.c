/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_jump.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): the jumps, the swaps and the save points this
 * runtime's rewritten slots reach (panel 255's R1, defects 2679 and 2702;
 * design.md §1.12, §4.19). Needs the files `rebind.c` names before it. */

#if defined(__APPLE__)

static uintptr_t hero_rebind_jmp_sp(const void *env) {
    return hero_rebind_strip_d(hero_rebind_word((const unsigned char *)env, HERO_REBIND_SP_WORD * 8) ^
                               atomic_load_explicit(&hero_rebind_key_k, memory_order_relaxed));
}

static uintptr_t hero_rebind_ctx_sp(const void *uc) {
    return hero_rebind_strip_d(hero_rebind_word((const unsigned char *)uc, HERO_REBIND_CTX_SP));
}

/* Whether `setcontext` would jump to `uc`: it refuses a context whose size
 * word is 0, and returns, and nothing must be judged left by a jump that does
 * not happen. */
static bool hero_rebind_ctx_valid(const void *uc) {
    return uc != NULL && hero_rebind_word((const unsigned char *)uc, HERO_REBIND_CTX_SIZE_WORD) != 0;
}

/* Route A outside every handler: the target's record asked, nothing read of
 * any buffer; a record naming an entry is a save made inside a handler, and a
 * jump to it now re-enters that handler's frames, whether or not its bytes
 * could be fingerprinted (a refusal is the safe side of a stale record). */
static void hero_rebind_reenter_by_record(HeroSignalStack *own, const void *buf) {
    HeroRebindRecord *r = hero_rebind_record(buf);
    if (r == NULL || atomic_load(&r->entry) == 0) return;
    uint64_t serial = 0;
    if (!hero_rebind_push(own, 0, 0, 0, 2, &serial)) own->lost += 1;
}

/* A jump of the setjmp family to `env`: its target decoded at every jump,
 * inside a handler or not (one word of a buffer the C library reads next), so
 * a landing in frames already left is seen wherever it happens. */
static void hero_rebind_jump(const void *env) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || env == NULL) return;
    bool standing = hero_rebind_standing(own);
    if (hero_rebind_decoding(HERO_REBIND_JMP)) {
        uintptr_t t = hero_rebind_jmp_sp(env);
        if (standing) hero_rebind_landing(own, t);
        if (standing) hero_rebind_prune_to(own, t, true);
        hero_rebind_reenter(own, t);
    } else if (standing) {
        hero_rebind_jump_by_record(own, env);
    } else {
        hero_rebind_reenter_by_record(own, env);
    }
}

/* Rule 6 for `setcontext` to `uc`: a target on a coroutine's stack (a known
 * range) leaves the entries on that range above it and holds every other
 * until its handler ends; any other target is a jump. */
static void hero_rebind_set(const void *uc) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || !hero_rebind_ctx_valid(uc)) return;
    bool standing = hero_rebind_standing(own);
    if (hero_rebind_decoding(HERO_REBIND_CTX)) {
        uintptr_t t = hero_rebind_ctx_sp(uc);
        hero_rebind_range_entered(t);
        if (standing) {
            hero_rebind_landing(own, t);
            uintptr_t lo = 0, hi = 0;
            if (hero_rebind_known_range(t, &lo, &hi)) {
                hero_rebind_prune_to(own, t, false);
                for (int i = 0; i < own->depth; i += 1) {
                    if (!own->entries[i].dead) own->entries[i].held = 1;
                }
            } else {
                hero_rebind_prune_to(own, t, true);
            }
        }
        hero_rebind_reenter(own, t);
    } else if (standing) {
        hero_rebind_jump_by_record(own, uc);
    } else {
        hero_rebind_reenter_by_record(own, uc);
    }
}

/* Rule 5: a swap made while a handler stands suspends every standing entry
 * until this frame is resumed, so the wrapper is no tail call; its marks live
 * in its own frame, and only the entries it marked are released, by serial,
 * when the swap returns here (or fails). The target is judged only for a
 * landing in frames already left (rule 6). */
__attribute__((noinline)) static int hero_rebind_swapcontext(void *from, const void *to) {
    HeroSignalStack *own = hero_rebind_own();
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    uintptr_t entered = 0;
    if (own != NULL && from != NULL && hero_rebind_ctx_valid(to)) {
        if (hero_rebind_decoding(HERO_REBIND_CTX)) {
            entered = hero_rebind_ctx_sp(to);
            hero_rebind_range_entered(entered);
            hero_rebind_landing(own, entered);
        }
        for (int i = 0; i < own->depth && n < HERO_SIG_DEPTH; i += 1) {
            HeroRebindEntry *e = &own->entries[i];
            if (e->dead) continue;
            atomic_fetch_add_explicit(&e->suspended, 1, memory_order_relaxed);
            marks[n] = e->serial;
            gens[n] = e->gen;
            n += 1;
        }
        if (hero_rebind_decoding(HERO_REBIND_CTX)) {
            hero_rebind_reenter(own, hero_rebind_ctx_sp(to));
        } else {
            hero_rebind_note(from);
            if (hero_rebind_standing(own)) {
                hero_rebind_jump_by_record(own, to);
            } else {
                hero_rebind_reenter_by_record(own, to);
            }
        }
    }
    int r = atomic_load_explicit(&hero_rebind_real_swapcontext, memory_order_relaxed)(from, to);
    /* The context switched to came back unchanged: nothing saved a point in
     * it, so its coroutine ended (or left without a save) and the range holds
     * no frames of it any more (defect 2706, `rebind_entries.c`). */
    if (entered != 0 && r == 0 && hero_rebind_ctx_sp(to) == entered) hero_rebind_range_ended(entered);
    if (own != NULL) {
        for (int j = 0; j < n; j += 1) {
            for (int i = 0; i < own->depth; i += 1) {
                HeroRebindEntry *e = &own->entries[i];
                if (!e->dead && e->serial == marks[j]) {
                    int was = atomic_load_explicit(&e->suspended, memory_order_relaxed);
                    while (e->gen == gens[j] && was > 0 &&
                           !atomic_compare_exchange_weak_explicit(&e->suspended, &was, was - 1, memory_order_relaxed,
                                                                  memory_order_relaxed)) {
                    }
                    break;
                }
            }
        }
    }
    return r;
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
    hero_rebind_set(context);
    return atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
}

/* Route A's save points, rewritten only for a family whose decode did not
 * verify: each records, then reaches the C library's own by a tail call, so
 * it saves this function's caller and returns twice into it. */
static int hero_rebind_setjmp(int *env) {
    hero_rebind_note(env);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_setjmp, memory_order_relaxed)(env);
}

static int hero_rebind__setjmp(int *env) {
    hero_rebind_note(env);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real__setjmp, memory_order_relaxed)(env);
}

static int hero_rebind_sigsetjmp(int *env, int save) {
    hero_rebind_note(env);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_sigsetjmp, memory_order_relaxed)(env, save);
}

static int hero_rebind_getcontext(void *context) {
    hero_rebind_note(context);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_getcontext, memory_order_relaxed)(context);
}

/* `makecontext` names its context's stack, which becomes a known one (rule
 * 7), and under route A for contexts its record goes. Its arguments are
 * variadic, which `musttail` refuses, so the wrapper is assembly: the argument
 * registers kept, the stack noted, then a branch to the C library's own with
 * the stack as the caller left it. */
void hero_rebind_made_context(const void *context);
void hero_rebind_made_context(const void *context) {
    hero_rebind_known_stack(context);
    if (!hero_rebind_decoding(HERO_REBIND_CTX)) hero_rebind_forget(context);
}

#if defined(__aarch64__)
__attribute__((naked)) static void hero_rebind_makecontext(void *context, void (*start)(void), int count, ...) {
    __asm__ volatile("stp x29, x30, [sp, #-48]!\n\t"
                     "mov x29, sp\n\t"
                     "stp x0, x1, [sp, #16]\n\t"
                     "str x2, [sp, #32]\n\t"
                     "bl _hero_rebind_made_context\n\t"
                     "ldp x0, x1, [sp, #16]\n\t"
                     "ldr x2, [sp, #32]\n\t"
                     "ldp x29, x30, [sp], #48\n\t"
                     "adrp x16, _hero_rebind_real_makecontext@GOTPAGE\n\t"
                     "ldr x16, [x16, _hero_rebind_real_makecontext@GOTPAGEOFF]\n\t"
                     "ldr x16, [x16]\n\t"
                     "br x16");
}
#elif defined(__x86_64__)
__attribute__((naked)) static void hero_rebind_makecontext(void *context, void (*start)(void), int count, ...) {
    __asm__ volatile("push %rbp\n\tmov %rsp, %rbp\n\t"
                     "push %rdi\n\tpush %rsi\n\tpush %rdx\n\tpush %rcx\n\tpush %r8\n\tpush %r9\n\tpush %rax\n\t"
                     "sub $8, %rsp\n\t"
                     "call _hero_rebind_made_context\n\t"
                     "add $8, %rsp\n\t"
                     "pop %rax\n\tpop %r9\n\tpop %r8\n\tpop %rcx\n\tpop %rdx\n\tpop %rsi\n\tpop %rdi\n\tpop %rbp\n\t"
                     "mov _hero_rebind_real_makecontext@GOTPCREL(%rip), %r11\n\t"
                     "jmp *(%r11)");
}
#else
#error parts/rebind.c names no makecontext wrapper for this machine
#endif

#endif

/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigjump_save.c — the C library's own save points and jumps, and what
 * each tells `parts/sigentries.c` of the handlers a thread is inside (panel
 * 255's R1 and R3, defects 2679 and 2702; design.md §1.12). Needs
 * `sigjump_record.c`.
 *
 * The save points are `setjmp`, `_setjmp`, `__sigsetjmp` and `getcontext`; the
 * jumps `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk` (the fortified
 * one) and `setcontext`; and `swapcontext`, which saves and jumps. A save
 * point records only where route A stands, then reaches the C library's own
 * by a branch. A jump of the setjmp family and a `setcontext` judge their
 * target before they jump (`hero_jmp_jump`, `hero_jmp_set`, rules 4 to 6); a
 * `swapcontext` suspends every standing entry until it returns in its own
 * frame (`hero_jmp_swap`, rule 5). `makecontext`'s wrapper, which learns the
 * known stacks, is `sigjump_make.c`. */

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

/* A jump of the setjmp family to `env`: its target decoded at every jump,
 * inside a handler or not (one word of a buffer the C library reads next), so
 * a landing in frames already left is seen wherever it happens. Where the
 * decode did not verify, route A's record judges it. */
static void hero_jmp_jump(const void *env) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || env == NULL) return;
    bool standing = hero_sig_standing(own);
    if (!hero_jmp_recording()) {
        uintptr_t t = hero_jmp_sp_of((const struct __jmp_buf_tag *)env);
        if (standing) {
            hero_sig_landing(own, t);
            hero_sig_prune_to(own, t, true);
        }
        hero_sig_reenter(own, t);
    } else if (standing) {
        hero_jmp_by_record(own, env);
    } else {
        hero_jmp_reenter_by_record(own, env);
    }
}

/* Rule 6 for `setcontext` to `ucp`: a target on a coroutine's stack (a known
 * range) leaves the entries on that range above it and holds every other
 * until its handler ends, whether or not a context was saved inside it that
 * the runtime saw; any other target is a jump. */
static void hero_jmp_set(const ucontext_t *ucp) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || ucp == NULL) return;
    bool standing = hero_sig_standing(own);
    if (!hero_ctx_recording()) {
        uintptr_t t = hero_jmp_context_sp(ucp);
        if (standing) {
            hero_sig_landing(own, t);
            uintptr_t lo = 0, hi = 0;
            if (hero_sig_known_range(t, &lo, &hi)) {
                hero_sig_prune_to(own, t, false);
                for (int i = 0; i < own->depth; i += 1) {
                    if (!own->entries[i].dead) own->entries[i].held = 1;
                }
            } else {
                hero_sig_prune_to(own, t, true);
            }
        }
        hero_sig_reenter(own, t);
    } else if (standing) {
        hero_jmp_by_record(own, ucp);
    } else {
        hero_jmp_reenter_by_record(own, ucp);
    }
}

/* THE SAVE POINTS: under A the frame noted, then the C library's own reached
 * by a branch; under C the branch alone. The names are parenthesised so
 * glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag));
    }
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    if (hero_ctx_recording()) hero_jmp_note(ucp, HERO_JMP_CONTEXT_BYTES);
    hero_ctx_get_fn real = atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    __attribute__((musttail)) return real(ucp);
}

/* THE JUMPS. Each finds the C library's own first, as a save point does. */
void(longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_longjmp, memory_order_relaxed), env, val);
}

void(_longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real__longjmp, memory_order_relaxed), env, val);
}

void(siglongjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_siglongjmp, memory_order_relaxed), env, val);
}

void __longjmp_chk(struct __jmp_buf_tag env[1], int val);
void __longjmp_chk(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_chk, memory_order_relaxed), env, val);
}

int(setcontext)(const ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_set(ucp);
    return atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
}

/* Rule 5: a swap made while a handler stands suspends every standing entry
 * until this frame is resumed, so the wrapper is no tail call; its marks live
 * in its own frame, and only the entries it marked are released, by serial,
 * when the swap returns here (or fails). The target is judged only for a
 * landing in frames already left (rule 6). The C library saves into `oucp` at
 * the call below and the other context returns here through it. */
__attribute__((noinline, no_sanitize("address"))) int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    hero_jmp_find();
    HeroSignalStack *own = hero_sig_own();
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    if (own != NULL && oucp != NULL && ucp != NULL) {
        bool field = !hero_ctx_recording();
        uintptr_t t = field ? hero_jmp_context_sp(ucp) : 0;
        if (field) hero_sig_landing(own, t);
        for (int i = 0; i < own->depth && n < HERO_SIG_DEPTH; i += 1) {
            HeroSigEntry *e = &own->entries[i];
            if (e->dead) continue;
            atomic_fetch_add_explicit(&e->suspended, 1, memory_order_relaxed);
            marks[n] = e->serial;
            gens[n] = e->gen;
            n += 1;
        }
        if (field) {
            hero_sig_reenter(own, t);
        } else {
            hero_jmp_note(oucp, HERO_JMP_CONTEXT_BYTES);
            if (hero_sig_standing(own)) {
                hero_jmp_by_record(own, ucp);
            } else {
                hero_jmp_reenter_by_record(own, ucp);
            }
        }
    }
    int r = atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
    if (n > 0) {
        for (int j = 0; j < n; j += 1) {
            for (int i = 0; i < own->depth; i += 1) {
                HeroSigEntry *e = &own->entries[i];
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

#endif

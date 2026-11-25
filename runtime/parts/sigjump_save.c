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
 * 255's R1 and R3, defects 2679, 2702 and 2745; design.md §1.12). Needs
 * `sigjump_record.c`.
 *
 * The save points are `setjmp`, `_setjmp`, `__sigsetjmp` and `getcontext`; the
 * jumps `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk` (the fortified
 * one) and `setcontext`; and `swapcontext`, which saves and jumps. A save
 * point records only where route A stands, then reaches the C library's own
 * by a branch. A jump of the setjmp family and a `setcontext` judge their
 * target before they jump (`hero_jmp_jump`, `hero_jmp_set`, rules 4 to 6),
 * the decoded stack pointer on route C, the trusted record's frame on route
 * A, by the same rules (defect 2745); a
 * `swapcontext` suspends every standing entry until it returns in its own
 * frame (`hero_jmp_swap`, rule 5). `makecontext`'s wrapper, which learns the
 * known stacks, is `sigjump_make.c`. */

#if !defined(_WIN32) && !defined(__APPLE__)

/* Whether the save points must record: only where the key is not verified. */
static bool hero_jmp_recording(void) { return atomic_load_explicit(&hero_jmp_decoding, memory_order_acquire) == 0; }
static bool hero_ctx_recording(void) { return atomic_load_explicit(&hero_ctx_decoding, memory_order_acquire) == 0; }

#if defined(HERO_JMP_DECODES) && !defined(HERO_JMP_MAKES)
#error the placement below is route A's: where `makecontext` is not wrapped, neither family decodes
#endif

/* WHERE `makecontext` IS NOT WRAPPED (glibc's architectures other than aarch64
 * and x86-64; defect d-lane-d2745-2) no stack `makecontext` hands out is known,
 * so a coroutine carved from the thread's own stack is that stack to rule 4,
 * and a handler's jump into it was judged to leave the handler, which a
 * target above it on that stack does: the program's function then ran inside
 * the signal (three rows of 2702 and 2705, simulated by `HERO_JMP_NO_DECODE`
 * with `HERO_JMP_NO_MAKE` on Linux arm64). What this runtime does see is its
 * own `swapcontext`: from the switch until it returns in its frame, the
 * thread runs on the stack the swap entered, a coroutine's, and only that
 * frame's return brings it back where it was (`elsewhere`). A save made there
 * names a target on a stack a context switch entered and this runtime never
 * sees end, so a jump to it from a handler leaves no entry but by leaving the
 * alternate stack the entry was pushed on, and every entry it keeps is told
 * as ONSTACK, as route C tells a jump onto a coroutine's stack. */
#if !defined(HERO_JMP_MAKES)
static void hero_jmp_prune_away(HeroSignalStack *own, uintptr_t t) {
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead || hero_sig_pinned(e)) continue;
        if (e->alt_hi > e->alt_lo && !hero_sig_holds(e->alt_lo, e->alt_hi, t)) {
            hero_sig_drop(own, i);
        } else if (e->unsure == 0) {
            e->unsure = 3;
        }
    }
}
#endif

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
 * decode did not verify, route A's trusted record gives the target, judged
 * the same way (defect 2745). A jump that leaves every handler the thread
 * stood in is asked whether it abandons this runtime's allocator
 * (`parts/sigguard.c`, panel 258's R1). */
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
    } else {
        uintptr_t t = 0;
        bool back = false;
        if (hero_jmp_recorded(own, env, standing, &t, &back)) {
#if !defined(HERO_JMP_MAKES)
            /* The jump lands where its save was made, so the thread runs
             * there: inside a swap of its own, or back where it was. */
            own->elsewhere = hero_jmp_saved_away(own, env) ? 1 : 0;
            if (own->elsewhere != 0) {
                if (standing) hero_jmp_prune_away(own, t);
                hero_jmp_landed(own, t, back);
                if (standing && !hero_sig_standing(own)) hero_signal_jump_checked();
                return;
            }
#endif
            if (standing) {
                hero_sig_landing(own, t);
                hero_sig_prune_to(own, t, true);
            }
            hero_jmp_landed(own, t, back);
        }
    }
    if (standing && !hero_sig_standing(own)) hero_signal_jump_checked();
}

/* Rule 6 for `setcontext` to `ucp`: a target on a coroutine's stack (a known
 * range) leaves the entries on that range above it and holds every other
 * until its handler ends, whether or not a context was saved inside it that
 * the runtime saw; any other target is a jump. */
static void hero_jmp_set(const ucontext_t *ucp) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || ucp == NULL) return;
    bool standing = hero_sig_standing(own);
    uintptr_t t = 0;
    bool back = false;
    bool route_c = !hero_ctx_recording();
    if (route_c) {
        t = hero_jmp_context_sp(ucp);
        hero_sig_range_entered(t);
    } else {
        hero_sig_range_switched(ucp);
        if (!hero_jmp_recorded(own, ucp, standing, &t, &back)) {
            if (standing && !hero_sig_standing(own)) hero_signal_jump_checked();
            return;
        }
    }
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
    if (route_c) {
        hero_sig_reenter(own, t);
    } else {
        hero_jmp_landed(own, t, back);
    }
    if (standing && !hero_sig_standing(own)) hero_signal_jump_checked();
}

/* THE SAVE POINTS: under A the buffer noted with a frame below this one's
 * caller (`hero_jmp_below`, called here, so its frame lies under the stack
 * pointer the C library saves, the caller's), then the C library's own reached
 * by a branch; under C the branch alone. The names are parenthesised so
 * glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    if (hero_ctx_recording()) hero_jmp_note(ucp, HERO_JMP_CONTEXT_BYTES, hero_jmp_below());
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

/* A switch made from this frame leaves the range it lies on, if a swap into
 * that range waits to return (`sigentries.c`, the life of a range). */
int(setcontext)(const ucontext_t *ucp) {
    hero_jmp_find();
    hero_sig_range_left(hero_jmp_below());
    hero_jmp_set(ucp);
    return atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
}

/* Rule 5: a swap made while a handler stands suspends every standing entry
 * until this frame is resumed, so the wrapper is no tail call; its marks live
 * in its own frame, and only the entries it marked are released, by serial,
 * when the swap returns here (or fails). The target is judged only for a
 * landing in frames already left (rule 6). The C library saves into `oucp` at
 * the call below and the other context returns here through it. Once the
 * switch has returned the context switched to is read only where it provably
 * lives, and by the kernel (defect d-lane-d2745-1): the end of a range's
 * coroutine is what the switches made from it while this one waited say,
 * with that word (`hero_sig_range_returned`). */
__attribute__((noinline, no_sanitize("address"))) int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    hero_jmp_find();
    HeroSignalStack *own = hero_sig_own();
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    int watched = -1;
    unsigned seq = 0;
    uintptr_t entered = 0;
    hero_sig_range_left((uintptr_t)__builtin_frame_address(0));
    if (own != NULL && oucp != NULL && ucp != NULL) {
        bool field = !hero_ctx_recording();
        uintptr_t t = field ? hero_jmp_context_sp(ucp) : 0;
        bool back = false;
        bool known = field || hero_jmp_target(own, ucp, &t, &back);
        if (field) {
            hero_sig_range_entered(t);
            watched = hero_sig_range_watch(t, &seq);
            entered = t;
        } else {
            hero_sig_range_switched(ucp);
        }
        if (known) hero_sig_landing(own, t);
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
            /* The C library saves into `oucp` this frame's own stack pointer,
             * at the call below: `hero_jmp_below` called from here lies under
             * it. */
            hero_jmp_note(oucp, HERO_JMP_CONTEXT_BYTES, hero_jmp_below());
            if (known) {
                hero_jmp_landed(own, t, back);
            } else {
                hero_jmp_unfollowed(own, ucp, hero_sig_standing(own));
            }
        }
    }
#if !defined(HERO_JMP_MAKES)
    /* The thread runs on the stack the swap enters until this frame returns
     * (the placement above). */
    int away = own != NULL ? own->elsewhere : 0;
    if (own != NULL) own->elsewhere = 1;
#endif
    int r = atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
#if !defined(HERO_JMP_MAKES)
    if (own != NULL) own->elsewhere = away;
#endif
    /* Back in this frame: where no switch was made from the range entered
     * and the context did not move, its coroutine returned through `uc_link`
     * and the range holds no frames of it any more (defect 2706,
     * `sigentries.c`). */
    if (watched >= 0) {
        bool moved = hero_jmp_context_moved(own, ucp, entered);
        hero_sig_range_returned(watched, seq, (uintptr_t)__builtin_frame_address(0), r == 0, moved);
    }
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

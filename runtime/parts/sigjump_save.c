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

/* Route A's target for a switch to the context `ucp` (rules 4 to 7): the trusted
 * record's frame where one is; else, where `ucp` is the very context
 * `makecontext` was handed for a known range (slot `made`), the top of that
 * range, where `makecontext` started it, since any later save into it would
 * have left a record; else none, and false. Until defect d-lane-ctx-after-1 a
 * switch into a context `makecontext` made, whose record `makecontext` drops,
 * was told as a jump to a buffer the runtime holds no record of, in words
 * naming a copy, a refill or a full table the program has none of. */
static bool hero_jmp_switch_target(HeroSignalStack *own, const ucontext_t *ucp, int made, uintptr_t *t, bool *back) {
    if (hero_jmp_target(own, ucp, t, back)) return true;
    if (made < 0 || !hero_sig_range_top(made, t)) return false;
    *back = false;
    return true;
}

/* Under AddressSanitizer, the shadow of the frames a jump or a `setcontext`
 * leaves, cleared where it can be and never by asking ASan on a stack it does
 * not know (`parts/rebind_jump.c`'s `hero_rebind_leaving`, the reason): on
 * the thread's own stack and the alternate stack `__asan_handle_no_return`;
 * on a range `makecontext` was handed, from here to the range's top, by this
 * runtime; elsewhere nothing. Until defect d-lane-sig-family-8 every jump
 * asked ASan, and on a coroutine's stack ASan wrote its warning into a correct
 * program's stderr. */
__attribute__((noinline)) static void hero_jmp_leaving(void) {
#if defined(HERO_JMP_ASAN)
    uintptr_t here = (uintptr_t)__builtin_frame_address(0), lo = 0, hi = 0, al = 0, ah = 0;
    HeroSignalStack *own = hero_sig_own();
    bool thread = own != NULL && here >= own->lo && here < own->hi;
    if (!thread && hero_sig_known_range(here, &lo, &hi)) {
        __asan_unpoison_memory_region((const void *)here, (size_t)(hi - here));
    } else if (own == NULL || thread || (hero_sig_alt_now(&al, &ah) && here >= al && here < ah)) {
        __asan_handle_no_return();
    }
#endif
}

__attribute__((no_sanitize("address"))) static _Noreturn void
hero_jmp_go(hero_jmp_jump_fn real, struct __jmp_buf_tag *env, int val) {
    hero_jmp_leaving();
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
    hero_sig_relived(own, (uintptr_t)__builtin_frame_address(0));
    bool standing = hero_sig_standing(own);
    if (!hero_jmp_recording()) {
        uintptr_t t = hero_jmp_sp_of((const struct __jmp_buf_tag *)env);
        hero_signal_switch_left(own, t);
        if (standing) {
            hero_sig_landing(own, t);
            hero_sig_prune_to(own, t, true);
        }
        hero_sig_reenter(own, t);
    } else {
        uintptr_t t = 0;
        bool back = false;
        if (hero_jmp_recorded(own, env, standing, &t, &back)) {
            hero_signal_switch_left(own, t);
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
__attribute__((noinline)) static void hero_jmp_set(const ucontext_t *ucp) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || ucp == NULL) return;
    hero_sig_relived(own, (uintptr_t)__builtin_frame_address(0));
    bool standing = hero_sig_standing(own);
    uintptr_t t = 0;
    bool back = false;
    bool route_c = !hero_ctx_recording();
    if (route_c) {
        t = hero_jmp_context_sp(ucp);
        hero_sig_range_entered(t);
    } else {
        int made = hero_sig_range_switched(ucp);
        if (!hero_jmp_switch_target(own, ucp, made, &t, &back)) {
            hero_jmp_unfollowed(own, ucp, standing);
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

/* A save point's frame: the thread runs there (defect 2855), which
 * overwrites any span of a handler's frames left by a jump from that frame
 * up. One read of the thread's slot where no span stands. */
static void hero_jmp_saving(void) {
    HeroSignalStack *own = hero_sig_own();
    if (own == NULL || (own->left_n == 0 && own->depth == 0)) return;
    uintptr_t below = hero_jmp_below();
    if (own->left_n > 0) hero_sig_relived(own, below);
    if (own->depth > 0) hero_sig_saved_at(own, below);
}

/* THE SAVE POINTS: under A the buffer noted with a frame below this one's
 * caller (`hero_jmp_below`, called here, so its frame lies under the stack
 * pointer the C library saves, the caller's), then the C library's own reached
 * by a branch; under C the branch alone. The names are parenthesised so
 * glibc's macros of them stay out. */
int(setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    hero_jmp_saving();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real_setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(_setjmp)(struct __jmp_buf_tag env[1]) {
    hero_jmp_find();
    hero_jmp_saving();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_save_fn real = atomic_load_explicit(&hero_jmp_real__setjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env);
}

int(__sigsetjmp)(struct __jmp_buf_tag env[1], int savemask) {
    hero_jmp_find();
    hero_jmp_saving();
    if (hero_jmp_recording()) {
        hero_jmp_note(env, (uint32_t)sizeof(struct __jmp_buf_tag), hero_jmp_below());
    }
    hero_jmp_sigsave_fn real = atomic_load_explicit(&hero_jmp_real_sigsetjmp, memory_order_relaxed);
    __attribute__((musttail)) return real(env, savemask);
}

int(getcontext)(ucontext_t *ucp) {
    hero_jmp_find();
    hero_jmp_saving();
    if (hero_ctx_recording()) hero_jmp_note(ucp, HERO_JMP_CONTEXT_BYTES, hero_jmp_below());
    hero_ctx_get_fn real = atomic_load_explicit(&hero_ctx_real_get, memory_order_relaxed);
    __attribute__((musttail)) return real(ucp);
}

/* THE JUMPS. Each finds the C library's own first, as a save point does.
 * Under AddressSanitizer none is instrumented (defect d-lane-sig-family-10):
 * clang puts a call to `__asan_handle_no_return` before every call of a
 * function that does not return, `hero_jmp_go` here, and on a coroutine's
 * stack, which ASan was never told of, ASan clears nothing there and writes
 * its warning into a correct program's stderr; what the jump leaves is
 * cleared by `hero_jmp_leaving`, as Darwin's are (`parts/rebind_jump.c`). */
__attribute__((no_sanitize("address"))) void(longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_longjmp, memory_order_relaxed), env, val);
}

__attribute__((no_sanitize("address"))) void(_longjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real__longjmp, memory_order_relaxed), env, val);
}

__attribute__((no_sanitize("address"))) void(siglongjmp)(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_siglongjmp, memory_order_relaxed), env, val);
}

void __longjmp_chk(struct __jmp_buf_tag env[1], int val);
__attribute__((no_sanitize("address"))) void __longjmp_chk(struct __jmp_buf_tag env[1], int val) {
    hero_jmp_find();
    hero_jmp_jump(env);
    hero_jmp_go(atomic_load_explicit(&hero_jmp_real_chk, memory_order_relaxed), env, val);
}

/* The switch of contexts, of `call`, from `from` (none for `setcontext`) to
 * `to`, that the wrapper whose frame is `frame` begins: refused where the
 * handler the thread runs now interrupted a switch naming either, else noted
 * until it is seen to end, its note opened by the wrapper before anything
 * else (defect 2871, panel 263's gap, `parts/sigguard.c`). */
static void hero_jmp_switch_begins(HeroSignalStack *own, const char *call, uintptr_t frame, const void *from,
                                   const void *to) {
    HeroSigSwitch next = {frame, 0, 0, (uintptr_t)from, (uintptr_t)to};
    if (!hero_sig_target_range(own, frame, &next.lo, &next.hi)) next.lo = next.hi = 0;
    hero_signal_switch_begin(own, call, &next);
}

/* `sigjump_make.c`: a context `makecontext` made, about to run first, has
 * its function's return sent through `hero_ctx_end` (defect 2868). */
static void hero_ctx_watch_end(const ucontext_t *ucp);

/* A `setcontext` into a context `makecontext` made, that nothing has run,
 * sends its function's return through `hero_ctx_end` first, so its range's
 * end is seen (`sigentries.c`, the life of a range). No
 * redzones in this frame (defect 2853, `parts/rebind_jump.c`'s reason): what
 * it judges runs in `hero_jmp_set`'s, which returns before the switch, and
 * what the switch leaves is cleared as a jump's is (`hero_jmp_leaving`). The
 * C library's own is called, never reached by a tail call, so its frames link
 * to this one while it switches (defect 2871); it returns only where it
 * refused the context. */
__attribute__((noinline, no_sanitize("address"))) int(setcontext)(const ucontext_t *ucp) {
    HeroSignalStack *own = hero_sig_own();
    HeroSigSwitch outer;
    if (own != NULL) {
        hero_signal_switch_open(own, (uintptr_t)__builtin_frame_address(0), 0, (uintptr_t)ucp, &outer);
    } else {
        hero_signal_switch_none(&outer);
    }
    hero_jmp_find();
    if (own != NULL) {
        hero_jmp_switch_begins(own, "setcontext", (uintptr_t)__builtin_frame_address(0), NULL, ucp);
    }
    hero_jmp_set(ucp);
    if (!hero_ctx_recording()) hero_ctx_watch_end(ucp);
    hero_jmp_leaving();
    int r = atomic_load_explicit(&hero_ctx_real_set, memory_order_relaxed)(ucp);
    if (own != NULL) hero_signal_switch_back(own, &outer, (uintptr_t)__builtin_frame_address(0));
    return r;
}

/* Rule 5: a swap made while a handler stands suspends every standing entry
 * until this frame is resumed, so the wrapper is no tail call; its marks live
 * in its own frame, and only the entries it marked are released, by serial,
 * when the swap returns here (or fails). The target is judged only for a
 * landing in frames already left (rule 6). The C library saves into `oucp` at
 * the call below and the other context returns here through it. A context
 * `makecontext` made, that nothing has run, has its function's return sent
 * through `hero_ctx_end` first, so its range's end is seen (defect 2868);
 * nothing of the context switched to is read once the switch has returned
 * (defect d-lane-d2745-1: the program may have given it back). It carries what the C
 * library's declaration writes after its parameters (`HERO_CTX_SWAP_RETURN`,
 * `sigjump.c`), written before it, where a definition takes attributes. */
__attribute__((noinline, no_sanitize("address"))) HERO_CTX_SWAP_RETURN
int(swapcontext)(ucontext_t *restrict oucp, const ucontext_t *restrict ucp) {
    HeroSignalStack *own = hero_sig_own();
    HeroSigSwitch outer;
    if (own != NULL) {
        hero_signal_switch_open(own, (uintptr_t)__builtin_frame_address(0), (uintptr_t)oucp, (uintptr_t)ucp,
                                &outer);
    } else {
        hero_signal_switch_none(&outer);
    }
    hero_jmp_find();
    if (own != NULL) {
        hero_jmp_switch_begins(own, "swapcontext", (uintptr_t)__builtin_frame_address(0), oucp, ucp);
    }
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    hero_sig_relived(own, (uintptr_t)__builtin_frame_address(0));
    hero_sig_saved_at(own, (uintptr_t)__builtin_frame_address(0));
    if (own != NULL && oucp != NULL && ucp != NULL) {
        bool field = !hero_ctx_recording();
        uintptr_t t = field ? hero_jmp_context_sp(ucp) : 0;
        bool back = false;
        bool known = field;
        if (field) {
            hero_sig_range_entered(t);
        } else {
            known = hero_jmp_switch_target(own, ucp, hero_sig_range_switched(ucp), &t, &back);
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
    if (!hero_ctx_recording()) hero_ctx_watch_end(ucp);
    int r = atomic_load_explicit(&hero_ctx_real_swap, memory_order_relaxed)(oucp, ucp);
    if (own != NULL) hero_signal_switch_back(own, &outer, (uintptr_t)__builtin_frame_address(0));
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

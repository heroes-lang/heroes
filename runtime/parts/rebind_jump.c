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
 * runtime's rewritten slots reach (panel 255's R1, defects 2679, 2702 and
 * 2745; design.md §1.12, §4.19). Needs the files `rebind.c` names before it. */

#if defined(__APPLE__)

/* UNDER ADDRESSSANITIZER A REWRITE THAT DOES NOT RETURN HAS NO REDZONES IN
 * ITS FRAME (defect 2853): instrumented, its frame kept redzones around its
 * locals that only its return would clear, and a jump or a `setcontext` left
 * them in the shadow of the thread's stack, where the next frames read as
 * overflowing a local of a frame that is gone (`stack-buffer-overflow ... in
 * frame #0 hero_rebind_setcontext`, a correct fibre program under `--sanitize`
 * on this Mac). So `setcontext`, the three jumps and the swap (whose context
 * may be abandoned) are `no_sanitize("address")`, and what they judge runs in
 * frames of its own (`hero_rebind_set`, `hero_rebind_jump`, never inlined)
 * that return, clearing theirs, before the switch.
 *
 * AND THE SHADOW OF THE FRAMES THE SWITCH LEAVES IS CLEARED WHERE IT CAN BE,
 * NEVER BY ASKING ASAN ON A STACK IT DOES NOT KNOW (`hero_rebind_leaving`):
 * on the thread's own stack and the alternate stack, which ASan knows,
 * `__asan_handle_no_return` clears from the leaving frame to the stack's top,
 * as ASan's own interceptors do; on a range `makecontext` was handed, which
 * ASan was never told of, this runtime clears from the leaving frame to the
 * range's top itself; on a stack neither knows, nothing is cleared, as ASan
 * would clear nothing there. Asked on a coroutine's stack, ASan clears nothing
 * and writes `WARNING: ASan is ignoring requested __asan_handle_no_return`
 * into a correct program's stderr: from step 199e's call in `setcontext`
 * (defect d-lane-sig-family-7, the CI's Darwin `--sanitize` leg on 77eb38534)
 * and from ASan's interceptor of each jump (d-lane-sig-family-8), which is
 * why the jumps are the C library's own (`rebind_image.c`). */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define HERO_REBIND_ASAN 1
void __asan_handle_no_return(void);
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#endif
#endif

__attribute__((noinline)) static void hero_rebind_leaving(void) {
#if defined(HERO_REBIND_ASAN)
    uintptr_t here = (uintptr_t)__builtin_frame_address(0), lo = 0, hi = 0, al = 0, ah = 0;
    HeroSignalStack *own = hero_rebind_own();
    bool thread = own != NULL && here >= own->lo && here < own->hi;
    if (!thread && hero_rebind_known_range(here, &lo, &hi)) {
        __asan_unpoison_memory_region((const void *)here, (size_t)(hi - here));
    } else if (own == NULL || thread || (hero_rebind_alt_now(&al, &ah) && here >= al && here < ah)) {
        __asan_handle_no_return();
    }
#endif
}

static uintptr_t hero_rebind_jmp_sp(const void *env) {
    return hero_rebind_strip_d(hero_rebind_word((const unsigned char *)env, HERO_REBIND_SP_WORD * 8) ^
                               atomic_load_explicit(&hero_rebind_key_k, memory_order_relaxed));
}

static uintptr_t hero_rebind_ctx_sp(const void *uc) {
    return hero_rebind_strip_d(hero_rebind_word((const unsigned char *)uc, HERO_REBIND_CTX_SP));
}

/* THE END OF A COROUTINE IS SEEN WHERE ITS FUNCTION RETURNS (defect 2868).
 * Darwin's `makecontext` keeps the context in x19, the function munged by the
 * process's pointer token in x20, and `_ctx_start` in the link register's slot:
 * `_ctx_start` calls the function, then `_ctx_done`, which switches to the
 * context's `uc_link` unseen. At the first switch this file sees into a
 * context `makecontext` made, whose link register still names `_ctx_start`
 * (`hero_rebind_watch_end`), the function moves to x21 and x20 names
 * `hero_rebind_ctx_tramp`, munged by the same token: the trampoline calls the
 * function with every argument as `_ctx_start` set it (the registers and the
 * stack as it left them), then tells the range its coroutine ended, makes the
 * same change to the context `_ctx_done` switches to next and enters the range
 * that one lands on, and returns into `_ctx_start`, which goes on as it would
 * have. `_ctx_start`'s address and the token are read before `main` from a
 * context made there (`rebind_decode.c`); the context is written only at a
 * switch into it, which the C library reads next, so it lives. Until then the
 * end was judged by what the switches made from the range and the context
 * switched to did not show, and a coroutine yielding by a switch the runtime
 * never sees, saving into another context, was judged ended while alive. */
__attribute__((used, visibility("hidden"))) void hero_rebind_ctx_ended(uintptr_t sp, const void *uc);

#if defined(__aarch64__)
__attribute__((naked)) static void hero_rebind_ctx_tramp(void) {
    __asm__ volatile("mov x22, x30\n\t"
                     "blr x21\n\t"
                     "mov x0, sp\n\t"
                     "mov x1, x19\n\t"
                     "bl _hero_rebind_ctx_ended\n\t"
                     "mov x30, x22\n\t"
                     "ret");
}

static void hero_rebind_watch_end(const void *uc) {
    uintptr_t start = atomic_load_explicit(&hero_rebind_ctx_start, memory_order_acquire);
    uintptr_t lo = 0, hi = 0;
    if (start == 0 || uc == NULL) return;
    unsigned char *c = (unsigned char *)(uintptr_t)uc;
    /* Written only where its registers lie inside it, as the C library's own
     * `makecontext` puts them: a context whose machine context the program
     * keeps elsewhere is left as it is, and its coroutine's end unseen. */
    if (hero_rebind_word(c, HERO_REBIND_CTX_MCONTEXT_WORD) != (uintptr_t)uc + HERO_REBIND_CTX_MCONTEXT) return;
    if (!hero_rebind_known_range(hero_rebind_ctx_sp(uc), &lo, &hi)) return;
    if (hero_rebind_strip_i(hero_rebind_word(c, HERO_REBIND_CTX_LR)) != start) return;
    uintptr_t token = atomic_load_explicit(&hero_rebind_ctx_token, memory_order_relaxed);
    uintptr_t fn = hero_rebind_word(c, HERO_REBIND_CTX_X20) ^ token;
    if (fn == (uintptr_t)hero_rebind_ctx_tramp) return;
    uintptr_t tramp = (uintptr_t)hero_rebind_ctx_tramp ^ token;
    memcpy(c + HERO_REBIND_CTX_X21, &fn, sizeof fn);
    memcpy(c + HERO_REBIND_CTX_X20, &tramp, sizeof tramp);
}

void hero_rebind_ctx_ended(uintptr_t sp, const void *uc) {
    hero_rebind_range_ended(sp);
    const ucontext_t *link = uc != NULL ? ((const ucontext_t *)uc)->uc_link : NULL;
    if (link == NULL) return;
    hero_rebind_watch_end(link);
    hero_rebind_range_entered(hero_rebind_ctx_sp(link));
}
#else
static void hero_rebind_watch_end(const void *uc) { (void)uc; }
void hero_rebind_ctx_ended(uintptr_t sp, const void *uc) {
    (void)sp;
    (void)uc;
}
#endif

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

/* A jump to `buf` no trusted record follows: every entry kept, marked unsure
 * inside a handler; outside every handler, a re-entry where its record names
 * one. */
static void hero_rebind_unfollowed(HeroSignalStack *own, const void *buf, bool standing) {
    if (standing) {
        hero_rebind_mark_unsure(own);
    } else {
        hero_rebind_reenter_by_record(own, buf);
    }
}

/* Route A's target for a switch to the context `uc` (rules 4 to 7): the trusted
 * record's frame where one is; else, where `uc` is the very context
 * `makecontext` was handed for a known range (slot `made`), the top of that
 * range, where `makecontext` started it, since any later save into it would
 * have left a record; else none, and false (defect d-lane-ctx-after-1: such a
 * switch, whose record `makecontext` drops, was told as a jump to a buffer the
 * runtime holds no record of). */
static bool hero_rebind_switch_target(HeroSignalStack *own, const void *uc, int made, uintptr_t *t, bool *back) {
    if (hero_rebind_target(own, uc, t, back)) return true;
    if (made < 0 || !hero_rebind_range_top(made, t)) return false;
    *back = false;
    return true;
}

/* The switch of contexts, of `call`, from `from` (none for `setcontext`) to
 * `to`, that the wrapper whose frame is `frame` begins: refused where the
 * handler the thread runs now interrupted a switch naming either, else noted
 * until it is seen to end, its note opened by the wrapper before anything
 * else (defect 2871, panel 263's gap, `parts/sigguard.c`). */
static void hero_rebind_switch_begins(HeroSignalStack *own, const char *call, uintptr_t frame, const void *from,
                                      const void *to) {
    HeroSigSwitch next = {frame, 0, 0, (uintptr_t)from, (uintptr_t)to};
    if (!hero_rebind_target_range(own, frame, &next.lo, &next.hi)) next.lo = next.hi = 0;
    hero_signal_switch_begin(own, call, &next);
}

/* A jump of the setjmp family to `env`: its target decoded at every jump,
 * inside a handler or not (one word of a buffer the C library reads next), so
 * a landing in frames already left is seen wherever it happens. Where the
 * decode did not verify, route A's trusted record gives the target, judged
 * the same way (defect 2745). A jump that leaves every handler the thread
 * stood in is asked whether it abandons this runtime's allocator
 * (`parts/sigguard.c`, panel 258's R1). */
__attribute__((noinline)) static void hero_rebind_jump(const void *env) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || env == NULL) return;
    hero_rebind_relived(own, (uintptr_t)__builtin_frame_address(0));
    bool standing = hero_rebind_standing(own);
    if (hero_rebind_decoding(HERO_REBIND_JMP)) {
        uintptr_t t = hero_rebind_jmp_sp(env);
        hero_signal_switch_left(own, t);
        if (standing) hero_rebind_landing(own, t);
        if (standing) hero_rebind_prune_to(own, t, true);
        hero_rebind_reenter(own, t);
    } else {
        uintptr_t t = 0;
        bool back = false;
        if (hero_rebind_target(own, env, &t, &back)) {
            hero_signal_switch_left(own, t);
            if (standing) hero_rebind_landing(own, t);
            if (standing) hero_rebind_prune_to(own, t, true);
            hero_rebind_landed(own, t, back);
        } else {
            hero_rebind_unfollowed(own, env, standing);
        }
    }
    if (standing && !hero_rebind_standing(own)) hero_signal_jump_checked();
}

/* Rule 6 for `setcontext` to `uc`: a target on a coroutine's stack (a known
 * range) leaves the entries on that range above it and holds every other
 * until its handler ends; any other target is a jump. */
__attribute__((noinline)) static void hero_rebind_set(const void *uc) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || !hero_rebind_ctx_valid(uc)) return;
    hero_rebind_relived(own, (uintptr_t)__builtin_frame_address(0));
    bool standing = hero_rebind_standing(own);
    uintptr_t t = 0;
    bool back = false;
    bool route_c = hero_rebind_decoding(HERO_REBIND_CTX);
    if (route_c) {
        t = hero_rebind_ctx_sp(uc);
        hero_rebind_range_entered(t);
    } else {
        int made = hero_rebind_range_switched(uc);
        if (!hero_rebind_switch_target(own, uc, made, &t, &back)) {
            hero_rebind_unfollowed(own, uc, standing);
            if (standing && !hero_rebind_standing(own)) hero_signal_jump_checked();
            return;
        }
    }
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
    if (route_c) {
        hero_rebind_reenter(own, t);
    } else {
        hero_rebind_landed(own, t, back);
    }
    if (standing && !hero_rebind_standing(own)) hero_signal_jump_checked();
}

/* Rule 5: a swap made while a handler stands suspends every standing entry
 * until this frame is resumed, so the wrapper is no tail call; its marks live
 * in its own frame, and only the entries it marked are released, by serial,
 * when the swap returns here (or fails). The target is judged only for a
 * landing in frames already left (rule 6). A context `makecontext` made, that
 * nothing has run, has its function called through `hero_rebind_ctx_tramp`
 * first, so its range's end is seen (defect 2868); nothing of the context
 * switched to is read once the switch has returned (defect d-lane-d2745-1:
 * the program may have given it back). */
__attribute__((noinline, no_sanitize("address"))) static int hero_rebind_swapcontext(void *from, const void *to) {
    HeroSignalStack *own = hero_rebind_own();
    HeroSigSwitch outer;
    if (own != NULL) {
        hero_signal_switch_open(own, (uintptr_t)__builtin_frame_address(0), (uintptr_t)from, (uintptr_t)to, &outer);
    } else {
        hero_signal_switch_none(&outer);
    }
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    uintptr_t entered = 0;
    if (own != NULL) {
        hero_rebind_switch_begins(own, "swapcontext", (uintptr_t)__builtin_frame_address(0), from, to);
    }
    hero_rebind_relived(own, (uintptr_t)__builtin_frame_address(0));
    hero_rebind_saved_at(own, (uintptr_t)__builtin_frame_address(0));
    if (own != NULL && from != NULL && hero_rebind_ctx_valid(to)) {
        bool field = hero_rebind_decoding(HERO_REBIND_CTX);
        uintptr_t t = 0;
        bool back = false;
        bool known = field;
        if (field) {
            entered = hero_rebind_ctx_sp(to);
            hero_rebind_range_entered(entered);
            hero_rebind_landing(own, entered);
        } else {
            known = hero_rebind_switch_target(own, to, hero_rebind_range_switched(to), &t, &back);
            if (known) hero_rebind_landing(own, t);
        }
        for (int i = 0; i < own->depth && n < HERO_SIG_DEPTH; i += 1) {
            HeroRebindEntry *e = &own->entries[i];
            if (e->dead) continue;
            atomic_fetch_add_explicit(&e->suspended, 1, memory_order_relaxed);
            marks[n] = e->serial;
            gens[n] = e->gen;
            n += 1;
        }
        if (field) {
            hero_rebind_reenter(own, entered);
        } else {
            /* The C library saves into `from` this frame's own stack pointer,
             * at the call below: `hero_rebind_below` called from here lies
             * under it. */
            hero_rebind_note(from, hero_rebind_below());
            if (known) {
                hero_rebind_landed(own, t, back);
            } else {
                hero_rebind_unfollowed(own, to, hero_rebind_standing(own));
            }
        }
    }
    if (hero_rebind_decoding(HERO_REBIND_CTX) && hero_rebind_ctx_valid(to)) hero_rebind_watch_end(to);
    int r = atomic_load_explicit(&hero_rebind_real_swapcontext, memory_order_relaxed)(from, to);
    if (own != NULL) hero_signal_switch_back(own, &outer, (uintptr_t)__builtin_frame_address(0));
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

/* THE FRAMES A JUMP ABANDONS ARE UNWOUND FIRST (panel 258's R2,
 * `parts/unwind.c`), where the jump is made outside every handler, by no
 * standing entry, and its target is decoded: the stack pointer it will
 * install, read before `hero_rebind_jump` judges the handlers, or 0 where the
 * unwind is not asked. */
static uintptr_t hero_rebind_unwinds(const void *env, uintptr_t *lo, uintptr_t *hi) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || env == NULL || hero_rebind_standing(own) || !hero_rebind_decoding(HERO_REBIND_JMP)) return 0;
    /* And only from the thread's own stack to a target on it, never from a
     * stack `makecontext` was handed or one the runtime does not know (a
     * coroutine's, leaving for good or switching by a jump). */
    uintptr_t here = (uintptr_t)__builtin_frame_address(0), t = hero_rebind_jmp_sp(env), rl = 0, rh = 0;
    if (here < own->lo || here >= own->hi || t <= here || t >= own->hi) return 0;
    if (hero_rebind_known_range(here, &rl, &rh)) return 0;
    *lo = own->lo;
    *hi = own->hi;
    return t;
}

/* `parts/unwind.c`'s reads of frames and code, by the kernel, as this route
 * reads a context's word (`hero_rebind_ctx_word`). */
static bool hero_unwind_peek(uintptr_t at, void *into, size_t size) {
    mach_vm_size_t got = 0;
    return mach_vm_read_overwrite(mach_task_self(), (mach_vm_address_t)at, size, (mach_vm_address_t)(uintptr_t)into,
                                  &got) == KERN_SUCCESS &&
           got == size;
}

/* The shadow `parts/unwind.c` clears before its unwind, as a jump's is. */
static void hero_unwind_clear(void) { hero_rebind_leaving(); }

/* The jump `parts/unwind.c`'s stop makes: the C library's own, after the
 * shadow of what it leaves is cleared, as every wrapper below does. */
__attribute__((no_sanitize("address"))) static _Noreturn void hero_unwind_go(int kind, void *env, int value) {
    hero_rebind_leaving();
    if (kind == HERO_UNWIND_SIGLONGJMP) {
        atomic_load_explicit(&hero_rebind_real_siglongjmp, memory_order_relaxed)((int *)env, value);
    } else if (kind == HERO_UNWIND__LONGJMP) {
        atomic_load_explicit(&hero_rebind_real__longjmp, memory_order_relaxed)((int *)env, value);
    } else {
        atomic_load_explicit(&hero_rebind_real_longjmp, memory_order_relaxed)((int *)env, value);
    }
    __builtin_unreachable();
}

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind_siglongjmp(int *env, int value) {
    uintptr_t lo = 0, hi = 0;
    uintptr_t unwinds = hero_rebind_unwinds(env, &lo, &hi);
    hero_rebind_jump(env);
    if (unwinds != 0) hero_unwind_jump(env, value, HERO_UNWIND_SIGLONGJMP, unwinds, lo, hi);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real_siglongjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind_longjmp(int *env, int value) {
    uintptr_t lo = 0, hi = 0;
    uintptr_t unwinds = hero_rebind_unwinds(env, &lo, &hi);
    hero_rebind_jump(env);
    if (unwinds != 0) hero_unwind_jump(env, value, HERO_UNWIND_LONGJMP, unwinds, lo, hi);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real_longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind__longjmp(int *env, int value) {
    uintptr_t lo = 0, hi = 0;
    uintptr_t unwinds = hero_rebind_unwinds(env, &lo, &hi);
    hero_rebind_jump(env);
    if (unwinds != 0) hero_unwind_jump(env, value, HERO_UNWIND__LONGJMP, unwinds, lo, hi);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real__longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

/* A context `makecontext` made, that nothing has run, has its function called
 * through `hero_rebind_ctx_tramp` first, so its range's end is seen
 * (`rebind_entries.c`, the life of a range). The C library's own is called, never reached by a tail call, so its frames link
 * to this one while it switches (defect 2871); it returns only where it
 * refused the context. */
__attribute__((noinline, no_sanitize("address"))) static int hero_rebind_setcontext(const void *context) {
    HeroSignalStack *own = hero_rebind_own();
    HeroSigSwitch outer;
    if (own != NULL) {
        hero_signal_switch_open(own, (uintptr_t)__builtin_frame_address(0), 0, (uintptr_t)context, &outer);
        hero_rebind_switch_begins(own, "setcontext", (uintptr_t)__builtin_frame_address(0), NULL, context);
    } else {
        hero_signal_switch_none(&outer);
    }
    hero_rebind_set(context);
    if (hero_rebind_decoding(HERO_REBIND_CTX) && hero_rebind_ctx_valid(context)) hero_rebind_watch_end(context);
    hero_rebind_leaving();
    int r = atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
    if (own != NULL) hero_signal_switch_back(own, &outer, (uintptr_t)__builtin_frame_address(0));
    return r;
}

/* The save points: on route A each records, with a frame below its caller's
 * stack pointer (`hero_rebind_below`, called here); on route C each only tells
 * where the thread runs (defect 2855, `hero_rebind_saving`); then it reaches
 * the C library's own by a tail call, so it saves this function's caller and
 * returns twice into it. */
static void hero_rebind_saving(const void *buf, int family) {
    HeroSignalStack *own = hero_rebind_own();
    uintptr_t below = hero_rebind_below();
    if (own != NULL && own->depth > 0) hero_rebind_saved_at(own, below);
    if (!hero_rebind_decoding(family)) {
        hero_rebind_note(buf, below);
        return;
    }
    if (own != NULL && own->left_n > 0) hero_rebind_relived(own, below);
}

static int hero_rebind_setjmp(int *env) {
    hero_rebind_saving(env, HERO_REBIND_JMP);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_setjmp, memory_order_relaxed)(env);
}

static int hero_rebind__setjmp(int *env) {
    hero_rebind_saving(env, HERO_REBIND_JMP);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real__setjmp, memory_order_relaxed)(env);
}

static int hero_rebind_sigsetjmp(int *env, int save) {
    hero_rebind_saving(env, HERO_REBIND_JMP);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_sigsetjmp, memory_order_relaxed)(env, save);
}

static int hero_rebind_getcontext(void *context) {
    hero_rebind_saving(context, HERO_REBIND_CTX);
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_getcontext, memory_order_relaxed)(context);
}

/* `makecontext` names its context's stack, which becomes a known one (rule
 * 7), and under route A for contexts its record goes and its range is held
 * (defect 2745). Its arguments are variadic, which `musttail`
 * refuses, so the wrapper is assembly: the argument registers kept, the stack
 * noted, then a branch to the C library's own with the stack as the caller
 * left it. */
void hero_rebind_made_context(const void *context);
void hero_rebind_made_context(const void *context) {
    const ucontext_t *uc = (const ucontext_t *)context;
    HeroSignalStack *own = hero_rebind_own();
    if (own != NULL && uc != NULL) {
        uintptr_t lo = (uintptr_t)uc->uc_stack.ss_sp;
        hero_signal_switch_remade(own, lo, lo + (uintptr_t)uc->uc_stack.ss_size);
    }
    /* Route A reads no switch's target, so it cannot see the range entered or
     * its coroutine end: the range is held from here on, and a target in it
     * lies on the coroutine (rule 7, defect 2745). */
    hero_rebind_known_stack(context, !hero_rebind_decoding(HERO_REBIND_CTX));
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

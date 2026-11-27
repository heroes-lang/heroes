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

/* The stack-pointer word of the context at `uc`, copied by the kernel wherever
 * it is mapped, so a context given back since is an answer and never a fault
 * (defect 2850); false where the kernel refuses the copy. */
static bool hero_rebind_ctx_word(const void *uc, uintptr_t *word) {
    const unsigned char *at = (const unsigned char *)uc + HERO_REBIND_CTX_SP;
    uintptr_t got_word = 0;
    mach_vm_size_t got = 0;
    if (uc == NULL || mach_vm_read_overwrite(mach_task_self(), (mach_vm_address_t)(uintptr_t)at, sizeof got_word,
                                             (mach_vm_address_t)(uintptr_t)&got_word, &got) != KERN_SUCCESS ||
        got != sizeof got_word) {
        return false;
    }
    *word = hero_rebind_strip_d(got_word);
    return true;
}

/* Whether the coroutine a swap switched to may still be alive once the switch
 * has returned: its context naming another stack pointer than `entered`, one
 * witness of its life beside the switches made from its range (a yield
 * through a switch this route never sees still moves it), copied by the
 * kernel wherever the context is mapped, never loaded (defect d-lane-d2745-1:
 * the word was loaded wherever the context lay, and a coroutine that gave it
 * back crashed the program inside the runtime); a context that cannot be read
 * proves no end (defect 2851: until then one that did not provably live was
 * not read, and its coroutine was judged ended). */
static bool hero_rebind_ctx_moved(const void *uc, uintptr_t entered) {
    uintptr_t word = 0;
    if (!hero_rebind_ctx_word(uc, &word)) return true;
    return word != entered;
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
 * until it is seen to end, the one it is made inside of kept in `prev`
 * (defect 2871, `parts/sigguard.c`). */
static void hero_rebind_switch_begins(HeroSignalStack *own, const char *call, uintptr_t frame, const void *from,
                                      const void *to, HeroSigSwitch *prev) {
    HeroSigSwitch next = {frame, 0, 0, (uintptr_t)from, (uintptr_t)to};
    if (!hero_rebind_target_range(own, frame, &next.lo, &next.hi)) next.lo = next.hi = 0;
    hero_signal_switch_begin(own, call, &next, prev);
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
 * landing in frames already left (rule 6). Once the switch has returned the
 * context switched to is copied by the kernel wherever it is mapped, never
 * loaded (defects d-lane-d2745-1 and 2851): the end of a range's coroutine is what the switches
 * made from it while this one waited say, with that word
 * (`hero_rebind_range_returned`). */
__attribute__((noinline, no_sanitize("address"))) static int hero_rebind_swapcontext(void *from, const void *to) {
    HeroSignalStack *own = hero_rebind_own();
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    uintptr_t entered = 0;
    int watched = -1;
    unsigned seq = 0;
    HeroSigSwitch outer = {0, 0, 0, 0, 0};
    if (own != NULL) {
        hero_rebind_switch_begins(own, "swapcontext", (uintptr_t)__builtin_frame_address(0), from, to, &outer);
    }
    hero_rebind_range_left((uintptr_t)__builtin_frame_address(0));
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
            watched = hero_rebind_range_watch(entered, &seq);
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
    int r = atomic_load_explicit(&hero_rebind_real_swapcontext, memory_order_relaxed)(from, to);
    if (own != NULL) hero_signal_switch_back(own, &outer, (uintptr_t)__builtin_frame_address(0));
    /* Back in this frame: where no switch was made from the range entered
     * and the context did not move, its coroutine returned through `uc_link`
     * and the range holds no frames of it any more (defect 2706,
     * `rebind_entries.c`). */
    if (watched >= 0) {
        bool moved = hero_rebind_ctx_moved(to, entered);
        hero_rebind_range_returned(watched, seq, (uintptr_t)__builtin_frame_address(0), r == 0, moved);
    }
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

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind_siglongjmp(int *env, int value) {
    hero_rebind_jump(env);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real_siglongjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind_longjmp(int *env, int value) {
    hero_rebind_jump(env);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real_longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

__attribute__((noreturn, no_sanitize("address"))) static void hero_rebind__longjmp(int *env, int value) {
    hero_rebind_jump(env);
    hero_rebind_leaving();
    atomic_load_explicit(&hero_rebind_real__longjmp, memory_order_relaxed)(env, value);
    __builtin_unreachable();
}

/* A switch made from this frame leaves the range it lies on, if a swap into
 * that range waits to return (`rebind_entries.c`, the life of a range). The
 * C library's own is called, never reached by a tail call, so its frames link
 * to this one while it switches (defect 2871); it returns only where it
 * refused the context. */
__attribute__((noinline, no_sanitize("address"))) static int hero_rebind_setcontext(const void *context) {
    HeroSignalStack *own = hero_rebind_own();
    HeroSigSwitch outer = {0, 0, 0, 0, 0};
    if (own != NULL) {
        hero_rebind_switch_begins(own, "setcontext", (uintptr_t)__builtin_frame_address(0), NULL, context, &outer);
    }
    hero_rebind_range_left(hero_rebind_below());
    hero_rebind_set(context);
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

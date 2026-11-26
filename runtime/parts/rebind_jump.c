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

/* A jump of the setjmp family to `env`: its target decoded at every jump,
 * inside a handler or not (one word of a buffer the C library reads next), so
 * a landing in frames already left is seen wherever it happens. Where the
 * decode did not verify, route A's trusted record gives the target, judged
 * the same way (defect 2745). A jump that leaves every handler the thread
 * stood in is asked whether it abandons this runtime's allocator
 * (`parts/sigguard.c`, panel 258's R1). */
static void hero_rebind_jump(const void *env) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || env == NULL) return;
    bool standing = hero_rebind_standing(own);
    if (hero_rebind_decoding(HERO_REBIND_JMP)) {
        uintptr_t t = hero_rebind_jmp_sp(env);
        if (standing) hero_rebind_landing(own, t);
        if (standing) hero_rebind_prune_to(own, t, true);
        hero_rebind_reenter(own, t);
    } else {
        uintptr_t t = 0;
        bool back = false;
        if (hero_rebind_target(own, env, &t, &back)) {
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
static void hero_rebind_set(const void *uc) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || !hero_rebind_ctx_valid(uc)) return;
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
__attribute__((noinline)) static int hero_rebind_swapcontext(void *from, const void *to) {
    HeroSignalStack *own = hero_rebind_own();
    uint64_t marks[HERO_SIG_DEPTH];
    unsigned gens[HERO_SIG_DEPTH];
    int n = 0;
    uintptr_t entered = 0;
    int watched = -1;
    unsigned seq = 0;
    hero_rebind_range_left((uintptr_t)__builtin_frame_address(0));
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

/* A switch made from this frame leaves the range it lies on, if a swap into
 * that range waits to return (`rebind_entries.c`, the life of a range). */
static int hero_rebind_setcontext(const void *context) {
    hero_rebind_range_left(hero_rebind_below());
    hero_rebind_set(context);
    return atomic_load_explicit(&hero_rebind_real_setcontext, memory_order_relaxed)(context);
}

/* Route A's save points, rewritten only for a family whose decode did not
 * verify: each records, with a frame below its caller's stack pointer
 * (`hero_rebind_below`, called here), then reaches the C library's own by a
 * tail call, so it saves this function's caller and returns twice into it. */
static int hero_rebind_setjmp(int *env) {
    hero_rebind_note(env, hero_rebind_below());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_setjmp, memory_order_relaxed)(env);
}

static int hero_rebind__setjmp(int *env) {
    hero_rebind_note(env, hero_rebind_below());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real__setjmp, memory_order_relaxed)(env);
}

static int hero_rebind_sigsetjmp(int *env, int save) {
    hero_rebind_note(env, hero_rebind_below());
    __attribute__((musttail)) return atomic_load_explicit(&hero_rebind_real_sigsetjmp, memory_order_relaxed)(env, save);
}

static int hero_rebind_getcontext(void *context) {
    hero_rebind_note(context, hero_rebind_below());
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

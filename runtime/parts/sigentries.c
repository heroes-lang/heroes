/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sigentries.c — the handlers a thread is inside, and what a jump, a
 * swap or a delivery does to them, on Linux and every POSIX system but
 * Darwin (panel 255's R3, defect 2702; design.md §1.12, §4.19). It carries
 * Darwin's rules, `parts/rebind_entries.c`'s, to the wrapper
 * `parts/siginstall.c` runs around every handler: the same seven rules, the
 * same shapes judged the same way, so that one golden holds on both platforms
 * (R3's union).
 *
 * WHY NOT ADDRESSES ALONE. A handler's entry used to be dropped by comparing
 * its frame's address with another frame's (`hero_sig_prune`): a coroutine's
 * stack, a `makecontext` stack, an alternate stack and the thread's own are
 * four stacks, and two addresses on two of them say nothing about which frame
 * is deeper. A Heroes function then ran while the signal was still being
 * handled (defect 2702, defect 2522's class). Here no two addresses on two
 * different stacks are ever compared, and no frame's address alone drops an
 * entry.
 *
 * THE RULES, as `parts/rebind.c` states them (4 to 7; 1 to 3 are the decode,
 * `parts/sigjump_decode.c`, and route A, `parts/sigjump_record.c`):
 * 4. A jump's target lies on a coroutine's stack where a range `makecontext`
 *    handed out holds it, else on the thread's own stack, else on the
 *    alternate stack registered now, else on a stack of unknown extent. An
 *    entry is left by a target on the same range that lies above it; an entry
 *    records the alternate stack it was pushed on.
 * 5. A swap made while a handler stands suspends every standing entry until
 *    it returns in its own frame; nothing drops a suspended entry but its own
 *    handler's end or a landing that resumes it. Neither the wrapper's push
 *    nor the thunk prunes by its own frame; a wrapper's end drops its own
 *    entry and the entries pushed after it that nothing suspends.
 * 6. A `setcontext` whose target lies on a coroutine's stack holds every
 *    standing entry until its handler ends (the coroutine may resume the
 *    handler through `uc_link`), whether or not a save was seen; any other
 *    `setcontext` is a jump. A jump that leaves the alternate stack an entry
 *    was pushed on leaves it. A jump that lands on the alternate stack from
 *    outside it, or below the frame making it on its own range, re-enters a
 *    handler's frames and stands as an entry of its own. A landing at or below
 *    a standing entry on its range is that handler running again.
 * 7. The known stacks are learned where `makecontext` is called (an assembly
 *    wrapper in `sigjump_save.c`: the C library reads `uc_stack` nowhere
 *    else), and nothing tells this route one has ended: a stale range is
 *    never asked about an entry, only about a target, where it costs a told
 *    refusal.
 * A jump or an entry this file cannot judge keeps every entry: a miss is a
 * told refusal, never Heroes code inside a signal. Every step is an atomic
 * load or store, a system call or a read of the thread's own record, so each
 * is async-signal-safe but `hero_sig_known_stack`, which is called from
 * `makecontext` and never from a signal. Needs `sigguard.c`. */

#if !defined(_WIN32) && !defined(__APPLE__)

/* The claimed thread's record, through the thread key `parts/sigguard.c`
 * makes, never a thread-local access (which may allocate inside a signal on
 * a thread nothing claimed); NULL on such a thread. */
static HeroSignalStack *hero_sig_own(void) {
    if (atomic_load_explicit(&hero_signal_keyed, memory_order_acquire) != 2) return NULL;
    return pthread_getspecific(atomic_load_explicit(&hero_signal_key, memory_order_relaxed));
}

/* Whether a stack pointer `sp` belongs to the stack `[lo, hi)`, judged by the
 * byte below it, where the next push lands: a context `makecontext` has just
 * made holds the very top of its range (glibc arm64, measured, `ap_1_0`),
 * which is that stack's and no other's, even where another stack's range
 * begins right there (two heap blocks side by side: `ctx-m3-o1-s1`). A
 * frame's address is judged as it stands, inside the stack. */
static bool hero_sig_holds(uintptr_t lo, uintptr_t hi, uintptr_t sp) {
    uintptr_t below = sp - 1;
    return below >= lo && below < hi;
}

/* THE STACKS `makecontext` HANDS OUT (rule 7): each context's stack, read from
 * the context the program hands `makecontext` (the C library reads and zeroes
 * the same range next), kept until a later one overlaps it. Written under a
 * spin word (`makecontext` is no signal-safe call), read slot by slot under a
 * sequence word: a slot read while written is skipped, so a target there is
 * judged by the thread's own and alternate stacks, or as unknown. */
#define HERO_SIG_STACKS 256
static _Atomic unsigned hero_sig_stack_seq[HERO_SIG_STACKS];
static _Atomic uintptr_t hero_sig_stack_lo[HERO_SIG_STACKS];
static _Atomic uintptr_t hero_sig_stack_hi[HERO_SIG_STACKS];
static _Atomic unsigned hero_sig_stack_next = 0;
static _Atomic int hero_sig_stack_writing = 0;
/* Whether `makecontext` ever named a stack: until it does, no range is asked
 * for (a jump outside every handler then costs one decode and two compares). */
static _Atomic int hero_sig_stack_any = 0;

/* THE LIFE OF A RANGE (defect 2706). A range `makecontext` was given may hold a
 * coroutine's frames only between two events this file sees: a switch INTO it
 * (a `swapcontext` or `setcontext` whose target lies there) and the end of that
 * switch's coroutine, which a `swapcontext` that returns with the context it
 * switched to unchanged saw (a coroutine that yields saves itself into a
 * context, so the one switched to changes; one that returns through
 * `uc_link`, or leaves by a `setcontext` that saves nothing, does not). A
 * range no switch entered, or whose coroutine ended, holds no coroutine's
 * frames: a save point in it was made by the thread's own code in memory a
 * helper's returned frame once lent `makecontext` (panel 257's `fp3` and the
 * critic's `n6`), and a jump to it is judged on the stack it lies on, the
 * thread's own, where a target above a handler leaves it. A switch into the
 * range clears its end again. */
#define HERO_SIG_RANGE_ENTERED 1
#define HERO_SIG_RANGE_ENDED 2
static _Atomic int hero_sig_stack_state[HERO_SIG_STACKS];

static void hero_sig_stack_put(int i, uintptr_t lo, uintptr_t hi) {
    atomic_fetch_add(&hero_sig_stack_seq[i], 1);
    atomic_store(&hero_sig_stack_lo[i], lo);
    atomic_store(&hero_sig_stack_hi[i], hi);
    atomic_fetch_add(&hero_sig_stack_seq[i], 1);
}

void hero_sig_known_stack(const void *context);
void hero_sig_known_stack(const void *context) {
    const ucontext_t *uc = (const ucontext_t *)context;
    if (uc == NULL) return;
    uintptr_t lo = (uintptr_t)uc->uc_stack.ss_sp;
    uintptr_t hi = lo + (uintptr_t)uc->uc_stack.ss_size;
    if (lo == 0 || hi <= lo) return;
    while (atomic_exchange(&hero_sig_stack_writing, 1) != 0) {
    }
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        uintptr_t l = atomic_load(&hero_sig_stack_lo[i]), h = atomic_load(&hero_sig_stack_hi[i]);
        if (h > l && l < hi && lo < h) hero_sig_stack_put(i, 0, 0);
    }
    unsigned at = atomic_fetch_add(&hero_sig_stack_next, 1) % HERO_SIG_STACKS;
    atomic_store_explicit(&hero_sig_stack_state[at], 0, memory_order_relaxed);
    hero_sig_stack_put((int)at, lo, hi);
    atomic_store_explicit(&hero_sig_stack_any, 1, memory_order_release);
    atomic_store(&hero_sig_stack_writing, 0);
}

/* The slot of the known range holding `at`, -1 where none does. */
static int hero_sig_range_slot(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    if (!atomic_load_explicit(&hero_sig_stack_any, memory_order_acquire)) return -1;
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        unsigned before = atomic_load(&hero_sig_stack_seq[i]);
        uintptr_t l = atomic_load(&hero_sig_stack_lo[i]), h = atomic_load(&hero_sig_stack_hi[i]);
        if ((before & 1) != 0 || atomic_load(&hero_sig_stack_seq[i]) != before) continue;
        if (h > l && hero_sig_holds(l, h, at)) {
            *lo = l;
            *hi = h;
            return i;
        }
    }
    return -1;
}

/* The known range holding `at`, where one does: every range `makecontext`
 * handed out and nothing overlapped since, whether or not it may hold frames. */
static bool hero_sig_known_range(uintptr_t at, uintptr_t *lo, uintptr_t *hi) { return hero_sig_range_slot(at, lo, hi) >= 0; }

/* The known range holding `at` that may hold a coroutine's frames now: a
 * switch entered it and none saw its coroutine end. */
static bool hero_sig_live_range(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    int i = hero_sig_range_slot(at, lo, hi);
    if (i < 0) return false;
    int state = atomic_load_explicit(&hero_sig_stack_state[i], memory_order_relaxed);
    return (state & HERO_SIG_RANGE_ENTERED) != 0 && (state & HERO_SIG_RANGE_ENDED) == 0;
}

/* A switch whose target's stack pointer is `at` enters the range holding it
 * (a `swapcontext` or `setcontext`, called before anything judges the target). */
static void hero_sig_range_entered(uintptr_t at) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_sig_range_slot(at, &lo, &hi);
    if (i >= 0) atomic_store_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_ENTERED, memory_order_relaxed);
}

/* A `swapcontext` to a context whose stack pointer was `at` returned with
 * that context unchanged: its coroutine ended (or left without a save). */
static void hero_sig_range_ended(uintptr_t at) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_sig_range_slot(at, &lo, &hi);
    if (i >= 0) atomic_fetch_or_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_ENDED, memory_order_relaxed);
}

/* The alternate stack registered now, where one is. */
static bool hero_sig_alt_now(uintptr_t *lo, uintptr_t *hi) {
    stack_t alt;
    if (sigaltstack(NULL, &alt) != 0 || (alt.ss_flags & SS_DISABLE) != 0 || alt.ss_size == 0) return false;
    *lo = (uintptr_t)alt.ss_sp;
    *hi = *lo + (uintptr_t)alt.ss_size;
    return true;
}

/* Rule 4: the range of the stack a jump's target lies on, a coroutine's
 * first (a target inside a range `makecontext` handed out lies on that
 * coroutine, even where the range was carved out of the thread's own stack:
 * shape A, but only while the range may hold the coroutine's frames,
 * defect 2706: a range no switch entered or whose coroutine ended is the
 * thread's own memory again), then the thread's own, then the alternate stack registered now;
 * false where its extent is unknown, which proves no entry left. */
static bool hero_sig_target_range(const HeroSignalStack *own, uintptr_t t, uintptr_t *lo, uintptr_t *hi) {
    if (hero_sig_live_range(t, lo, hi)) return true;
    if (hero_sig_holds(own->lo, own->hi, t)) {
        *lo = own->lo;
        *hi = own->hi;
        return true;
    }
    uintptr_t al = 0, ah = 0;
    if (hero_sig_alt_now(&al, &ah) && hero_sig_holds(al, ah, t)) {
        *lo = al;
        *hi = ah;
        return true;
    }
    return false;
}

/* An entry pinned by rule 5 or 6, which no address may drop. */
static bool hero_sig_pinned(HeroSigEntry *e) {
    return atomic_load_explicit(&e->suspended, memory_order_relaxed) > 0 || e->held;
}

/* `i` gone, and `depth` brought down past every gone entry at the top. One
 * store marks it, so a signal arriving between the two sees a gone entry
 * or a standing one, never a half-moved one. */
static void hero_sig_drop(HeroSignalStack *own, int i) {
    own->entries[i].dead = 1;
    atomic_signal_fence(memory_order_seq_cst);
    while (own->depth > 0 && own->entries[own->depth - 1].dead) {
        own->depth -= 1;
        atomic_signal_fence(memory_order_seq_cst);
    }
}

/* A new entry at `at`, `alt_lo..alt_hi` the alternate stack it lies on;
 * false where the record is full. */
static bool hero_sig_push(HeroSignalStack *own, uintptr_t at, uintptr_t alt_lo, uintptr_t alt_hi, int unsure,
                          uint64_t *serial) {
    if (own->depth >= HERO_SIG_DEPTH) return false;
    int i = own->depth;
    HeroSigEntry *e = &own->entries[i];
    own->serial += 1;
    e->at = at;
    e->serial = own->serial;
    e->alt_lo = alt_lo;
    e->alt_hi = alt_hi;
    atomic_store_explicit(&e->suspended, 0, memory_order_relaxed);
    e->gen = 0;
    e->held = 0;
    e->unsure = unsure;
    e->dead = 0;
    atomic_signal_fence(memory_order_seq_cst);
    own->depth = i + 1;
    atomic_signal_fence(memory_order_seq_cst);
    if (alt_hi > alt_lo) own->alt_seen = 1;
    *serial = e->serial;
    return true;
}

/* The handler of `e` runs again (a landing in its own frames, or a trusted
 * record naming it): no swap or `setcontext` suspends it any more, and a swap
 * made before returns late without releasing a later one's mark. */
static void hero_sig_resumed(HeroSigEntry *e) {
    atomic_store_explicit(&e->suspended, 0, memory_order_relaxed);
    e->gen += 1;
    e->held = 0;
}

/* A landing at `t` inside a handler still standing on `t`'s range (at or below
 * its wrapper's frame) runs that handler again (`sp_0`: a coroutine resumes the
 * handler that swapped to it by a jump into the handler's own buffer, not
 * through the swap). */
static void hero_sig_landing(HeroSignalStack *own, uintptr_t t) {
    uintptr_t lo = 0, hi = 0;
    if (own->depth == 0 || !hero_sig_target_range(own, t, &lo, &hi)) return;
    for (int i = 0; i < own->depth; i += 1) {
        HeroSigEntry *e = &own->entries[i];
        if (!e->dead && e->at >= lo && e->at < hi && e->at >= t) hero_sig_resumed(e);
    }
}

/* Whether any entry stands. */
static bool hero_sig_standing(const HeroSignalStack *own) { return own->depth > 0 || own->lost > 0; }

/* Rules 4 to 6 at a jump whose target's stack pointer is `t` (`by_jump`: the
 * jump never comes back, as `longjmp` and `setcontext` do not): an entry
 * nothing pins is left where the target lies on the entry's range above it,
 * or where the jump leaves the alternate stack the entry was pushed on. */
static void hero_sig_prune_to(HeroSignalStack *own, uintptr_t t, bool by_jump) {
    uintptr_t lo = 0, hi = 0;
    bool ranged = hero_sig_target_range(own, t, &lo, &hi);
    /* A jump onto a coroutine's stack that an entry survives is told as such
     * (`unsure` 3, defect 2705): the words say what the runtime judged. */
    uintptr_t clo = 0, chi = 0;
    bool oncoroutine = by_jump && hero_sig_live_range(t, &clo, &chi);
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead || hero_sig_pinned(e)) continue;
        bool above = ranged && e->at >= lo && e->at < hi && e->at <= t;
        bool left_alt = by_jump && e->alt_hi > e->alt_lo && !hero_sig_holds(e->alt_lo, e->alt_hi, t);
        if (above || left_alt) {
            hero_sig_drop(own, i);
        } else if (oncoroutine && e->unsure == 0) {
            e->unsure = 3;
        }
    }
}

/* Rule 6's last sentence, and its twin on one range: a jump that lands BELOW
 * the jumping frame on the range that frame stands on lands in frames this
 * thread already unwound past, and one that lands on the alternate stack from
 * outside it lands in frames only a signal's delivery puts there. Either is a
 * handler left earlier and resumed now (shape R: a handler left by a jump out
 * and jumped back into), or frames that are gone: it stands as an entry of its
 * own, unsure, until a jump above it, the end of a handler pushed before it,
 * or a jump leaving the alternate stack it lies on, proves it left. Every
 * correct jump lands above the frame that makes it, on its own range or on
 * another. */
static void hero_sig_reenter(HeroSignalStack *own, uintptr_t t) {
    uintptr_t here = (uintptr_t)__builtin_frame_address(0);
    uintptr_t lo = 0, hi = 0, al = 0, ah = 0;
    bool below = hero_sig_target_range(own, here, &lo, &hi) && hero_sig_holds(lo, hi, t) && t < here;
    /* The alternate stack is asked only where it can matter: a landing below,
     * whose entry records it, or a thread a handler once ran on it. */
    bool alt = (below || own->alt_seen) && hero_sig_alt_now(&al, &ah) && hero_sig_holds(al, ah, t);
    bool into_alt = alt && own->alt_seen && !(here >= al && here < ah);
    if (!below && !into_alt) return;
    /* A landing at or below a handler still standing on the target's range
     * resumes that handler's own frames (a coroutine swapping back into the
     * handler that swapped to it): the standing entry already refuses, in the
     * true words, and outlives anything pushed here. */
    if (hero_sig_target_range(own, t, &lo, &hi)) {
        for (int i = 0; i < own->depth; i += 1) {
            const HeroSigEntry *e = &own->entries[i];
            if (!e->dead && e->at >= lo && e->at < hi && e->at >= t) return;
        }
    }
    uint64_t serial = 0;
    if (!hero_sig_push(own, t, alt ? al : 0, alt ? ah : 0, 2, &serial)) own->lost += 1;
}

/* Every standing entry marked as one a jump this file could not follow may
 * have left: the refusal then says so. */
static void hero_sig_mark_unsure(HeroSignalStack *own) {
    for (int i = 0; i < own->depth; i += 1) {
        if (!own->entries[i].dead && own->entries[i].unsure == 0) own->entries[i].unsure = 1;
    }
}

/* The serial of the innermost entry standing, 0 for none. */
static uint64_t hero_sig_innermost(const HeroSignalStack *own) {
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        if (!own->entries[i].dead) return own->entries[i].serial;
    }
    return 0;
}

/* The thunk's question (`parts/sigguard.c`): any entry standing refuses, on
 * whatever stack and however pinned (a handler suspended by a swap is still
 * handling its signal), told as unsure where the innermost is marked so and
 * as switched where it is pinned by a swap or a `setcontext`. No prune by the
 * thunk's own frame (rule 5). */
static int hero_sig_inside(HeroSignalStack *own) {
    if (own == NULL || !hero_sig_standing(own)) return HERO_SIG_OUTSIDE;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead) continue;
        if (e->unsure) return e->unsure == 2 ? HERO_SIG_REENTERED : e->unsure == 3 ? HERO_SIG_ONSTACK : HERO_SIG_UNSURE;
        return hero_sig_pinned(e) ? HERO_SIG_SWITCHED : HERO_SIG_INSIDE;
    }
    return HERO_SIG_INSIDE;
}

#endif

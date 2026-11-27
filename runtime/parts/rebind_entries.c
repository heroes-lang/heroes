/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/rebind_entries.c — Darwin's route D-rebind, part of `parts/rebind.c`
 * (read its module doc first): the entries: the handlers this thread is
 * inside, and what a jump, a swap or a delivery does to them (rules 4 to 7)
 * (panel 255's R1, defects 2679 and 2702; design.md §1.12, §4.19). Needs the
 * files `rebind.c` names before it. */

#if defined(__APPLE__)

/* The claimed thread's record through the thread key, never a thread-local
 * access; NULL on a thread nothing claimed, where the thunk refuses anyway. */
static HeroSignalStack *hero_rebind_own(void) {
    if (atomic_load_explicit(&hero_signal_keyed, memory_order_acquire) != 2) return NULL;
    return pthread_getspecific(atomic_load_explicit(&hero_signal_key, memory_order_relaxed));
}

/* THE STACKS `makecontext` HANDS OUT (rule 7): each context's stack, read from
 * the context the program hands `makecontext` (the C library reads and zeroes
 * the same range next), kept until a later one overlaps it. Written under a
 * spin word (`makecontext` is no signal-safe call), read slot by slot under a
 * sequence word: a slot read while written is skipped, so a target there is
 * judged by the thread's own and alternate stacks, or as unknown. */
#define HERO_REBIND_STACKS 256
static _Atomic unsigned hero_rebind_stack_seq[HERO_REBIND_STACKS];
static _Atomic uintptr_t hero_rebind_stack_lo[HERO_REBIND_STACKS];
static _Atomic uintptr_t hero_rebind_stack_hi[HERO_REBIND_STACKS];
static _Atomic unsigned hero_rebind_stack_next = 0;
static _Atomic int hero_rebind_stack_writing = 0;
/* Whether `makecontext` ever named a stack: until it does, no range is asked
 * for (a jump outside every handler then costs one decode and two compares). */
static _Atomic int hero_rebind_stack_any = 0;

/* THE LIFE OF A RANGE (defect 2706). A range `makecontext` was given may hold a
 * coroutine's frames only between two events this file sees: a switch INTO it
 * (a `swapcontext` or `setcontext` whose target lies there) and the end of that
 * switch's coroutine, which is its function returning. Darwin's C library
 * calls the function from its own `_ctx_start`, which then calls `_ctx_done`
 * to switch to the context's `uc_link`, both unseen; so at the first switch
 * this file sees into a context `makecontext` made and nothing has run yet,
 * the function `_ctx_start` will call becomes `hero_rebind_ctx_tramp`
 * (`rebind_jump.c`), which calls the program's function and, once it has
 * returned, tells the range its coroutine ended (`hero_rebind_range_ended`).
 * A coroutine leaves its range alive by any switch or jump, seen or not, and
 * dead only by that return: no absence of a witness is read as an end
 * (defect 2868; until then the end was judged where a swap into the range
 * returned with no switch made from the range meanwhile and the context
 * switched to not moved, which a yield by a switch the runtime never sees,
 * saving into another context, also leaves). A range no switch entered, or
 * whose coroutine ended, holds no coroutine's frames: a save point in it was
 * made by the thread's own code in memory a helper's returned frame once lent
 * `makecontext` (panel 257's `fp3` and the critic's `n6`), and a jump to it is
 * judged on the stack it lies on, the thread's own, where a target above a
 * handler leaves it. A switch into the range clears its end again. A context
 * a switch this file never sees enters first keeps `_ctx_start`'s call, so
 * its coroutine's end is never seen: its range, once entered, stays so.
 *
 * A COROUTINE THE C LIBRARY STARTS THROUGH ANOTHER'S `uc_link` (defect 2850)
 * is entered by no switch the program makes (Darwin's `_ctx_done` switches to
 * it): the link is learned where `makecontext` reads it, a range whose
 * coroutine is entered holds every range down its chain of links entered with
 * it, a range made after the context naming it as well, and where the
 * linker's end is seen the context it links to is changed the same way and
 * the range it lands on entered, as `parts/sigentries.c` says. */
#define HERO_REBIND_RANGE_ENTERED 1
#define HERO_REBIND_RANGE_ENDED 2
/* WHERE CONTEXTS FALL TO ROUTE A (defect 2745) no switch's target is read and
 * no coroutine's end is seen, so a range is HELD from its `makecontext` on:
 * it may hold a coroutine's frames for as long as it stands. A switch to the
 * very context `makecontext` was handed for it (`hero_rebind_stack_ctx`, a
 * pointer compared, never read) enters it as route C's does; a range held and
 * never so entered is told in words of its own. */
#define HERO_REBIND_RANGE_HELD 4
static _Atomic int hero_rebind_stack_state[HERO_REBIND_STACKS];
static _Atomic uintptr_t hero_rebind_stack_ctx[HERO_REBIND_STACKS];
/* The context each range's `uc_link` named when `makecontext` was handed it
 * (0 for none). */
static _Atomic uintptr_t hero_rebind_stack_link[HERO_REBIND_STACKS];

static void hero_rebind_range_links(int i);

static void hero_rebind_stack_put(int i, uintptr_t lo, uintptr_t hi) {
    atomic_fetch_add(&hero_rebind_stack_seq[i], 1);
    atomic_store(&hero_rebind_stack_lo[i], lo);
    atomic_store(&hero_rebind_stack_hi[i], hi);
    atomic_fetch_add(&hero_rebind_stack_seq[i], 1);
}

void hero_rebind_known_stack(const void *context, bool held);
void hero_rebind_known_stack(const void *context, bool held) {
    const ucontext_t *uc = (const ucontext_t *)context;
    if (uc == NULL) return;
    uintptr_t lo = (uintptr_t)uc->uc_stack.ss_sp;
    uintptr_t hi = lo + (uintptr_t)uc->uc_stack.ss_size;
    if (lo == 0 || hi <= lo) return;
    while (atomic_exchange(&hero_rebind_stack_writing, 1) != 0) {
    }
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
        if (h > l && l < hi && lo < h) hero_rebind_stack_put(i, 0, 0);
        if (atomic_load(&hero_rebind_stack_ctx[i]) == (uintptr_t)context) atomic_store(&hero_rebind_stack_ctx[i], 0);
    }
    unsigned at = atomic_fetch_add(&hero_rebind_stack_next, 1) % HERO_REBIND_STACKS;
    int state = held ? HERO_REBIND_RANGE_HELD : 0;
    /* A context another range's `uc_link` names, that range entered: the C
     * library may start this one whenever that coroutine ends. */
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        if ((unsigned)i == at || atomic_load(&hero_rebind_stack_link[i]) != (uintptr_t)context) continue;
        int other = atomic_load_explicit(&hero_rebind_stack_state[i], memory_order_relaxed);
        if ((other & HERO_REBIND_RANGE_ENTERED) != 0 && (other & HERO_REBIND_RANGE_ENDED) == 0) {
            state |= HERO_REBIND_RANGE_ENTERED;
        }
    }
    atomic_store_explicit(&hero_rebind_stack_state[at], state, memory_order_relaxed);
    atomic_store(&hero_rebind_stack_ctx[at], (uintptr_t)context);
    atomic_store(&hero_rebind_stack_link[at], (uintptr_t)uc->uc_link);
    hero_rebind_stack_put((int)at, lo, hi);
    atomic_store_explicit(&hero_rebind_stack_any, 1, memory_order_release);
    atomic_store(&hero_rebind_stack_writing, 0);
    if ((state & HERO_REBIND_RANGE_ENTERED) != 0) hero_rebind_range_links((int)at);
}

/* The slot of the range `makecontext` was handed `context` for, -1 where none
 * stands. */
static int hero_rebind_slot_of(uintptr_t context) {
    if (context == 0) return -1;
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        if (atomic_load(&hero_rebind_stack_ctx[i]) != context) continue;
        if (atomic_load(&hero_rebind_stack_hi[i]) > atomic_load(&hero_rebind_stack_lo[i])) return i;
    }
    return -1;
}

/* The ranges down the chain of links from slot `i`, entered with it: the C
 * library starts each when the one before ends. A link already entered and not
 * ended, its own chain was entered when it was. */
static void hero_rebind_range_links(int i) {
    for (int step = 0; step < HERO_REBIND_STACKS; step += 1) {
        int next = hero_rebind_slot_of(atomic_load(&hero_rebind_stack_link[i]));
        if (next < 0 || next == i) return;
        int was = atomic_fetch_or_explicit(&hero_rebind_stack_state[next], HERO_REBIND_RANGE_ENTERED,
                                           memory_order_relaxed);
        if ((was & HERO_REBIND_RANGE_ENDED) != 0) {
            atomic_fetch_and_explicit(&hero_rebind_stack_state[next], ~HERO_REBIND_RANGE_ENDED, memory_order_relaxed);
        } else if ((was & HERO_REBIND_RANGE_ENTERED) != 0) {
            return;
        }
        i = next;
    }
}

/* The slot of the known range holding `at`, -1 where none does: a stack
 * pointer judged by the byte below it, where the next push lands, as
 * `sigentries.c`'s `hero_sig_holds` judges it, so the very top of a range is
 * that range's (a frame's address, inside its stack, reads the same either
 * way). Until defect 2868's repair this route judged `at` as it stands, and a
 * coroutine whose stack's top is 16-aligned returned from its function at
 * that very top, on no range: under `--sanitize` on this Mac, which aligns a
 * frame's array to 32, the range of a coroutine run to its end stayed entered
 * and a correct program was refused in ONSTACK's words (lane sig-family-3's
 * `re_0`, the stack `[0x16fdbde00, 0x16fdfde00)` and the end's stack pointer
 * `0x16fdfde00`, read in lldb). */
static int hero_rebind_range_slot(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    if (!atomic_load_explicit(&hero_rebind_stack_any, memory_order_acquire)) return -1;
    uintptr_t below = at - 1;
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        unsigned before = atomic_load(&hero_rebind_stack_seq[i]);
        uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
        if ((before & 1) != 0 || atomic_load(&hero_rebind_stack_seq[i]) != before) continue;
        if (h > l && below >= l && below < h) {
            *lo = l;
            *hi = h;
            return i;
        }
    }
    return -1;
}

/* The known range holding `at`, where one does: every range `makecontext`
 * handed out and nothing overlapped since, whether or not it may hold frames. */
static bool hero_rebind_known_range(uintptr_t at, uintptr_t *lo, uintptr_t *hi) { return hero_rebind_range_slot(at, lo, hi) >= 0; }

/* What the known range holding `at` may hold now: 0 no coroutine's frames,
 * `HERO_REBIND_RANGE_ENTERED` a coroutine a switch entered and none saw end,
 * `HERO_REBIND_RANGE_HELD` a coroutine route A cannot see entered or ended. */
static int hero_rebind_range_life(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    int i = hero_rebind_range_slot(at, lo, hi);
    if (i < 0) return 0;
    int state = atomic_load_explicit(&hero_rebind_stack_state[i], memory_order_relaxed);
    if ((state & HERO_REBIND_RANGE_ENTERED) != 0 && (state & HERO_REBIND_RANGE_ENDED) == 0) {
        return HERO_REBIND_RANGE_ENTERED;
    }
    return (state & HERO_REBIND_RANGE_HELD) != 0 ? HERO_REBIND_RANGE_HELD : 0;
}

/* The known range holding `at` that may hold a coroutine's frames now. */
static bool hero_rebind_live_range(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    return hero_rebind_range_life(at, lo, hi) != 0;
}

/* The top of the range that may hold a coroutine's frames now and holds the
 * frame at `frame`: where the frame walk ends (`parts/sigguard.c`, defect
 * 2852; Darwin's `makecontext` starts a coroutine's records at the range's
 * very top, measured on this Mac). */
static bool hero_rebind_coroutine_top(uintptr_t frame, uintptr_t *top) {
    uintptr_t lo = 0;
    return hero_rebind_live_range(frame, &lo, top);
}

/* THE KNOWN RANGE A FAULT AT `addr` MAY HAVE RUN OUT OF (defect 2870), for the
 * stack guard's handler (`parts/stack.c`), which asks no other and judges the
 * fault's witnesses itself: of the ranges `makecontext` was handed whose lowest
 * address lies above `addr`, with `sp` below their top, the one whose lowest
 * address lies nearest. Every known range, ended or not: the witnesses say a
 * stack ran out, and a range's life says only where a coroutine's frames may
 * be. Atomic loads alone, a slot read while written skipped. Unused under the
 * sanitizer, whose handler replaces the guard's. */
__attribute__((unused)) static bool hero_rebind_range_above(uintptr_t addr, uintptr_t sp, uintptr_t *lo, uintptr_t *hi) {
    if (!atomic_load_explicit(&hero_rebind_stack_any, memory_order_acquire)) return false;
    bool found = false;
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        unsigned before = atomic_load(&hero_rebind_stack_seq[i]);
        uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
        if ((before & 1) != 0 || atomic_load(&hero_rebind_stack_seq[i]) != before || h <= l) continue;
        if (addr >= l || sp >= h || (found && l >= *lo)) continue;
        *lo = l;
        *hi = h;
        found = true;
    }
    return found;
}

/* Route A's switch to `context`: the range `makecontext` was handed it for,
 * where one was, entered (a pointer compared, nothing of the context read);
 * that range's slot, -1 where none was. */
static int hero_rebind_range_switched(const void *context) {
    if (context == NULL || !atomic_load_explicit(&hero_rebind_stack_any, memory_order_acquire)) return -1;
    int made = -1;
    for (int i = 0; i < HERO_REBIND_STACKS; i += 1) {
        if (atomic_load(&hero_rebind_stack_ctx[i]) == (uintptr_t)context) {
            atomic_fetch_or_explicit(&hero_rebind_stack_state[i], HERO_REBIND_RANGE_ENTERED, memory_order_relaxed);
            made = i;
        }
    }
    if (made >= 0) hero_rebind_range_links(made);
    return made;
}

/* The top of the range in slot `i`, where a context `makecontext` made for it
 * starts; false where the slot is being written or names no range now. The
 * top itself is judged the range's by rule 4 only through the frame below it,
 * so the target given is the last word inside it. */
static bool hero_rebind_range_top(int i, uintptr_t *t) {
    unsigned before = atomic_load(&hero_rebind_stack_seq[i]);
    uintptr_t l = atomic_load(&hero_rebind_stack_lo[i]), h = atomic_load(&hero_rebind_stack_hi[i]);
    if ((before & 1) != 0 || atomic_load(&hero_rebind_stack_seq[i]) != before || h <= l) return false;
    *t = h - sizeof(uintptr_t);
    return true;
}

/* A switch whose target's stack pointer is `at` enters the range holding it
 * (a `swapcontext` or `setcontext`, called before anything judges the target). */
static void hero_rebind_range_entered(uintptr_t at) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_rebind_range_slot(at, &lo, &hi);
    if (i < 0) return;
    atomic_store_explicit(&hero_rebind_stack_state[i], HERO_REBIND_RANGE_ENTERED, memory_order_relaxed);
    hero_rebind_range_links(i);
}

/* The coroutine of the range holding `sp` returned from its function, seen by
 * `hero_rebind_ctx_tramp` on that range's stack (defect 2868): the range holds
 * no frames of it any more. */
static void hero_rebind_range_ended(uintptr_t sp) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_rebind_range_slot(sp, &lo, &hi);
    if (i >= 0) atomic_fetch_or_explicit(&hero_rebind_stack_state[i], HERO_REBIND_RANGE_ENDED, memory_order_relaxed);
}

/* The alternate stack registered now, where one is. */
static bool hero_rebind_alt_now(uintptr_t *lo, uintptr_t *hi) {
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
static bool hero_rebind_target_range(const HeroSignalStack *own, uintptr_t t, uintptr_t *lo, uintptr_t *hi) {
    if (hero_rebind_live_range(t, lo, hi)) return true;
    if (t >= own->lo && t < own->hi) {
        *lo = own->lo;
        *hi = own->hi;
        return true;
    }
    uintptr_t al = 0, ah = 0;
    if (hero_rebind_alt_now(&al, &ah) && t >= al && t < ah) {
        *lo = al;
        *hi = ah;
        return true;
    }
    return false;
}

/* An entry pinned by rule 5 or 6, which no address may drop. */
static bool hero_rebind_pinned(HeroRebindEntry *e) {
    return atomic_load_explicit(&e->suspended, memory_order_relaxed) > 0 || e->held;
}

/* `i` gone, and `depth` brought down past every gone entry at the top. One
 * store marks it, so a signal arriving between the two sees a gone entry or a
 * standing one, never a half-moved one. */
static void hero_rebind_drop(HeroSignalStack *own, int i) {
    own->entries[i].dead = 1;
    atomic_signal_fence(memory_order_seq_cst);
    while (own->depth > 0 && own->entries[own->depth - 1].dead) {
        own->depth -= 1;
        atomic_signal_fence(memory_order_seq_cst);
    }
}

/* THE FRAMES OF A HANDLER LEFT BY A JUMP (rule 6, defect 2855). A jump that
 * leaves a handler leaves its frames below its trampoline's frame, down to the
 * frame making the jump where that lies on the same stack, else to that
 * stack's bottom; they are kept as a span until the thread is seen running
 * there again (a save point, a jump, a swap or a handler's own frame inside
 * it), which overwrites the span from that frame up. A landing inside a span
 * is that handler re-entered: shape R, a handler left by a jump and jumped
 * back into, from its own stack or another (panel 255's `rr_2`, a residue
 * until this defect). Until this defect any landing below the frame making
 * the jump on that frame's stack was read as one, and a coroutine on a stack
 * carved from the thread's own, which a switch the runtime never sees had
 * entered, leaving for good by a `longjmp` to the save point its host made
 * below it (panel 259's `escape_carved`), was told it had re-entered a
 * handler the program does not have, and refused. */
static void hero_rebind_left_add(HeroSignalStack *own, uintptr_t lo, uintptr_t hi, uintptr_t slo, uintptr_t shi) {
    if (hi <= lo) return;
    for (int k = 0; k < own->left_n; k += 1) {
        if (own->left_slo[k] == slo && own->left_shi[k] == shi && lo <= own->left_hi[k] && own->left_lo[k] <= hi) {
            if (lo < own->left_lo[k]) own->left_lo[k] = lo;
            if (hi > own->left_hi[k]) own->left_hi[k] = hi;
            return;
        }
    }
    if (own->left_n < HERO_SIG_LEFT) {
        int k = own->left_n;
        own->left_lo[k] = lo;
        own->left_hi[k] = hi;
        own->left_slo[k] = slo;
        own->left_shi[k] = shi;
        atomic_signal_fence(memory_order_seq_cst);
        own->left_n = k + 1;
        return;
    }
    /* Full (defect d-lane-sig-family-4): a span is widened only over spans of
     * its own stack, where what lies between two of them is frames the thread
     * left too; until then the first span was widened to the hull of two
     * stacks, a static alternate stack and the thread's, and a correct
     * coroutine resumed on a stack mapped between them read as re-entering a
     * handler (panel 262's `wide`). First the nearest span of this one's stack
     * takes it; else the two nearest spans of one stack merge, freeing a slot;
     * else every span lies on a stack of its own, and the thread is marked. */
    int near = -1;
    uintptr_t gap = UINTPTR_MAX;
    for (int k = 0; k < own->left_n; k += 1) {
        if (own->left_slo[k] != slo || own->left_shi[k] != shi) continue;
        uintptr_t d = own->left_hi[k] <= lo ? lo - own->left_hi[k] : own->left_lo[k] >= hi ? own->left_lo[k] - hi : 0;
        if (d < gap) {
            gap = d;
            near = k;
        }
    }
    if (near >= 0) {
        if (lo < own->left_lo[near]) own->left_lo[near] = lo;
        if (hi > own->left_hi[near]) own->left_hi[near] = hi;
        return;
    }
    int a = -1, b = -1;
    gap = UINTPTR_MAX;
    for (int i = 0; i < own->left_n; i += 1) {
        for (int j = i + 1; j < own->left_n; j += 1) {
            if (own->left_slo[i] != own->left_slo[j] || own->left_shi[i] != own->left_shi[j]) continue;
            uintptr_t d = own->left_hi[i] <= own->left_lo[j]   ? own->left_lo[j] - own->left_hi[i]
                          : own->left_hi[j] <= own->left_lo[i] ? own->left_lo[i] - own->left_hi[j]
                                                               : 0;
            if (d < gap) {
                gap = d;
                a = i;
                b = j;
            }
        }
    }
    if (a < 0) {
        own->left_lost = 1;
        return;
    }
    if (own->left_lo[b] < own->left_lo[a]) own->left_lo[a] = own->left_lo[b];
    if (own->left_hi[b] > own->left_hi[a]) own->left_hi[a] = own->left_hi[b];
    own->left_lo[b] = lo;
    own->left_hi[b] = hi;
    own->left_slo[b] = slo;
    own->left_shi[b] = shi;
}

/* `e`'s frames, left by a jump made below `here` (its frame lies on the
 * entry's stack where the jump is made there). A re-entry's own entry holds
 * the landing itself, so its span reaches one byte above it. */
static void hero_rebind_left_by_jump(HeroSignalStack *own, const HeroRebindEntry *e, uintptr_t here) {
    uintptr_t slo = 0, shi = 0;
    if (e->alt_hi > e->alt_lo) {
        slo = e->alt_lo;
        shi = e->alt_hi;
    } else if (!hero_rebind_target_range(own, e->at, &slo, &shi)) {
        return;
    }
    uintptr_t lo = (here >= slo && here < shi) && here < e->at ? here : slo;
    hero_rebind_left_add(own, lo, e->unsure == 2 ? e->at + 1 : e->at, slo, shi);
}

/* The thread seen running at `frame`: every span its stack holds above the
 * frame runs again, from the frame up. */
static void hero_rebind_relived(HeroSignalStack *own, uintptr_t frame) {
    if (own == NULL) return;
    for (int k = own->left_n - 1; k >= 0; k -= 1) {
        if (!(frame >= own->left_slo[k] && frame < own->left_shi[k]) || frame >= own->left_hi[k]) continue;
        if (frame > own->left_lo[k]) {
            own->left_hi[k] = frame;
            continue;
        }
        int last = own->left_n - 1;
        own->left_lo[k] = own->left_lo[last];
        own->left_hi[k] = own->left_hi[last];
        own->left_slo[k] = own->left_slo[last];
        own->left_shi[k] = own->left_shi[last];
        atomic_signal_fence(memory_order_seq_cst);
        own->left_n = last;
    }
}

/* Whether `t` lies inside the frames of a handler left by a jump. */
static bool hero_rebind_left_holds(const HeroSignalStack *own, uintptr_t t) {
    for (int k = 0; k < own->left_n; k += 1) {
        if (t >= own->left_lo[k] && t < own->left_hi[k]) return true;
    }
    return false;
}

/* A new entry at `at`, `alt_lo..alt_hi` the alternate stack it lies on;
 * false where the record is full. */
static bool hero_rebind_push(HeroSignalStack *own, uintptr_t at, uintptr_t alt_lo, uintptr_t alt_hi, int unsure,
                             uint64_t *serial) {
    if (own->depth >= HERO_SIG_DEPTH) return false;
    int i = own->depth;
    HeroRebindEntry *e = &own->entries[i];
    own->serial += 1;
    e->at = at;
    e->serial = own->serial;
    e->alt_lo = alt_lo;
    e->alt_hi = alt_hi;
    atomic_store_explicit(&e->suspended, 0, memory_order_relaxed);
    e->gen = 0;
    e->held = 0;
    e->unsure = unsure;
    e->saved = 0;
    e->dead = 0;
    e->cut_from = 0;
    e->cut_to = 0;
    atomic_signal_fence(memory_order_seq_cst);
    own->depth = i + 1;
    atomic_signal_fence(memory_order_seq_cst);
    if (alt_hi > alt_lo) own->alt_seen = 1;
    *serial = e->serial;
    return true;
}

/* The handler of `e` runs again (a landing in its own frames, or a
 * trusted record naming it): no swap or `setcontext` suspends it any more,
 * and a swap made before returns late without releasing a later one's mark. */
static void hero_rebind_resumed(HeroRebindEntry *e) {
    atomic_store_explicit(&e->suspended, 0, memory_order_relaxed);
    e->gen += 1;
    e->held = 0;
}

/* A landing at `t` inside a handler still standing on `t`'s range (at
 * or below its trampoline's frame) runs that handler again (`sp_0`: a
 * coroutine resumes the handler that swapped to it by a jump into the
 * handler's own buffer, not through the swap). */
static void hero_rebind_landing(HeroSignalStack *own, uintptr_t t) {
    uintptr_t lo = 0, hi = 0;
    if (own->depth == 0 || !hero_rebind_target_range(own, t, &lo, &hi)) return;
    for (int i = 0; i < own->depth; i += 1) {
        HeroRebindEntry *e = &own->entries[i];
        if (!e->dead && e->at >= lo && e->at < hi && e->at >= t) hero_rebind_resumed(e);
    }
}

/* Whether any entry stands. */
static bool hero_rebind_standing(const HeroSignalStack *own) { return own->depth > 0 || own->lost > 0; }

/* Rules 4 to 6 at a jump whose target's stack pointer is `t` (`by_jump`: the
 * jump never comes back, as `longjmp` and `setcontext` do not): an entry
 * nothing pins is left where the target lies on the entry's range above it,
 * or where the jump leaves the alternate stack the entry was pushed on. */
/* A save point made at `frame` (the save hooks and the swap): every entry
 * standing whose own stack holds the frame, below the entry's, has had a save
 * made inside its handler's frames, which can switch back into them however
 * the handler is later left (defect 2872). An entry whose stack is not known
 * is marked too, so it is judged as before. */
static void hero_rebind_saved_at(HeroSignalStack *own, uintptr_t frame) {
    if (own == NULL) return;
    for (int i = 0; i < own->depth; i += 1) {
        HeroRebindEntry *e = &own->entries[i];
        if (e->dead || frame >= e->at) continue;
        uintptr_t lo = 0, hi = 0;
        if (e->alt_hi > e->alt_lo) {
            lo = e->alt_lo;
            hi = e->alt_hi;
        } else if (!hero_rebind_target_range(own, e->at, &lo, &hi)) {
            e->saved = 1;
            continue;
        }
        if (frame >= lo && frame < hi) e->saved = 1;
    }
}

/* Whether a jump from `here` to `t` leaves the handler of `e` by leaving the
 * stack its frames lie on: 1 where the jump is made inside the handler's
 * frames and lands on another stack, no save point having been made inside
 * those frames since the handler began, so nothing can switch back into them
 * but a landing its left span catches (rule 6); 2 where one was, which can;
 * 0 otherwise. Two shapes, one case, as on Linux (`parts/sigentries.c`): a
 * handler on a coroutine's stack landing off it (defect 2872, `ck`), and its
 * mirror, a handler on the thread's own stack landing on a coroutine's (lane
 * sig-family-3, `as_0`). A handler on an alternate stack is rule 6's. */
static int hero_rebind_off_stack(const HeroSignalStack *own, const HeroRebindEntry *e, uintptr_t here, uintptr_t t) {
    uintptr_t lo = 0, hi = 0, clo = 0, chi = 0;
    if (e->alt_hi > e->alt_lo) return 0;
    if (hero_rebind_live_range(e->at, &lo, &hi)) {
        if (t - 1 >= lo && t - 1 < hi) return 0;
    } else if (e->at >= own->lo && e->at < own->hi && hero_rebind_live_range(t, &clo, &chi)) {
        lo = own->lo;
        hi = own->hi;
    } else {
        return 0;
    }
    if (here < lo || here >= e->at) return 0;
    return e->saved ? 2 : 1;
}

static void hero_rebind_prune_to(HeroSignalStack *own, uintptr_t t, bool by_jump) {
    uintptr_t here = (uintptr_t)__builtin_frame_address(0);
    uintptr_t lo = 0, hi = 0;
    bool ranged = hero_rebind_target_range(own, t, &lo, &hi);
    /* A jump onto a coroutine's stack that an entry survives is told as such
     * (`unsure` 3, defect 2705): the words say what the runtime judged. */
    uintptr_t clo = 0, chi = 0;
    int life = by_jump ? hero_rebind_range_life(t, &clo, &chi) : 0;
    bool oncoroutine = life != 0;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroRebindEntry *e = &own->entries[i];
        if (e->dead || hero_rebind_pinned(e)) continue;
        bool above = ranged && e->at >= lo && e->at < hi && e->at <= t;
        bool left_alt = by_jump && e->alt_hi > e->alt_lo && !(t >= e->alt_lo && t < e->alt_hi);
        int off = by_jump ? hero_rebind_off_stack(own, e, here, t) : 0;
        if (above || left_alt || off == 1) {
            hero_rebind_left_by_jump(own, e, here);
            hero_rebind_drop(own, i);
        } else if (oncoroutine && e->unsure == 0) {
            e->unsure = life == HERO_REBIND_RANGE_ENTERED ? 3 : 4;
        } else if (off == 2 && e->unsure == 0) {
            e->unsure = 5;
        }
    }
}

/* Rule 6's last sentence: a jump that lands inside the frames of a handler a
 * jump left, where the thread has not run since (`hero_rebind_left_holds`), lands
 * in that handler's frames (shape R: a handler left by a jump out and jumped
 * back into, from its own stack or another), and one that lands on the
 * alternate stack from outside it lands in frames only a signal's delivery
 * puts there. Either stands as an entry of its own, unsure, until a jump above
 * it, the end of a handler pushed before it, or a jump leaving the alternate
 * stack it lies on, proves it left. A landing below the frame making the jump
 * elsewhere is no handler's (defect 2855): a coroutine on a stack carved from
 * the thread's own, entered by a switch the runtime never sees, lands below
 * itself when it leaves for good to its host's save point. */
static void hero_rebind_reentered(HeroSignalStack *own, uintptr_t t, bool below, uintptr_t here);

static void hero_rebind_reenter(HeroSignalStack *own, uintptr_t t) {
    uintptr_t here = (uintptr_t)__builtin_frame_address(0);
    bool held = hero_rebind_left_holds(own, t);
    uintptr_t lo = 0, hi = 0;
    if (!held && own->left_lost) held = hero_rebind_target_range(own, here, &lo, &hi) && (t >= lo && t < hi) && t < here;
    hero_rebind_reentered(own, t, held, here);
}

/* The landing at `t` judged once it is known whether it lies below the frame
 * making the jump (`here`, on the jumping stack): route C knows it by the two
 * addresses; route A, whose `t` is only a bound of the target, by its record
 * naming a handler no longer standing (`rebind_record.c`). */
static void hero_rebind_reentered(HeroSignalStack *own, uintptr_t t, bool below, uintptr_t here) {
    uintptr_t lo = 0, hi = 0, al = 0, ah = 0;
    /* The alternate stack is asked only where it can matter: a landing below,
     * whose entry records it, or a thread a handler once ran on it. */
    bool alt = (below || own->alt_seen) && hero_rebind_alt_now(&al, &ah) && t >= al && t < ah;
    bool into_alt = alt && own->alt_seen && !(here >= al && here < ah);
    if (!below && !into_alt) return;
    /* A landing at or below a handler still standing on the target's range
     * resumes that handler's own frames (a coroutine swapping back into the
     * handler that swapped to it): the standing entry already refuses, in the
     * true words, and outlives anything pushed here. */
    if (hero_rebind_target_range(own, t, &lo, &hi)) {
        for (int i = 0; i < own->depth; i += 1) {
            const HeroRebindEntry *e = &own->entries[i];
            if (!e->dead && e->at >= lo && e->at < hi && e->at >= t) return;
        }
    }
    uint64_t serial = 0;
    if (!hero_rebind_push(own, t, alt ? al : 0, alt ? ah : 0, 2, &serial)) own->lost += 1;
}

/* Every standing entry marked as one a jump this file could not follow may
 * have left: the refusal then says so. */
static void hero_rebind_mark_unsure(HeroSignalStack *own) {
    for (int i = 0; i < own->depth; i += 1) {
        if (!own->entries[i].dead && own->entries[i].unsure == 0) own->entries[i].unsure = 1;
    }
}

typedef void (*hero_rebind_catcher)(int, siginfo_t *, void *);

/* Route A's settle, below. */
static void hero_rebind_settle(void);

/* The kernel's call of the catcher, made here in its place, so its C type is
 * not checked, for `parts/siginstall.c`'s wrapper's reason; nor is its frame
 * instrumented by AddressSanitizer (it leaves by `__sigreturn`; panel 251).
 * Under route A the buffer saved last is fingerprinted first, by the kernel:
 * a handler that refills it by a save no slot sees, then
 * jumps to it, finds it no longer reads as its fingerprint (`t_unseen0`,
 * `x_unseen0` ran inside the signal when the fingerprint waited for the next
 * save or jump); on the decode path nothing is ever pending, and it returns at
 * once. Its entry pushed (rule 5: no prune by its own frame), the catcher
 * called, then its entry dropped with every entry pushed after it that nothing
 * pins. */
__attribute__((no_sanitize("function", "address"))) static void hero_rebind_tramp(hero_rebind_catcher catcher, int style,
                                                                                  int sig, siginfo_t *info,
                                                                                  void *context, uintptr_t token) {
    (void)style;
    hero_rebind_settle();
    HeroSignalStack *own = hero_rebind_own();
    uint64_t mine = 0;
    bool lost = false;
    if (own != NULL) {
        uintptr_t me = (uintptr_t)__builtin_frame_address(0);
        hero_rebind_relived(own, me);
        bool cut = hero_signal_in_switch(own, context);
        uintptr_t al = 0, ah = 0;
        if (!hero_rebind_alt_now(&al, &ah) || me < al || me >= ah) al = ah = 0;
        if (!hero_rebind_push(own, me, al, ah, 0, &mine)) {
            own->lost += 1;
            lost = true;
        }
        hero_signal_mark_cut(own, mine, cut);
    }
    catcher(sig, info, context);
    if (own != NULL) {
        if (mine != 0) {
            for (int i = own->depth - 1; i >= 0; i -= 1) {
                HeroRebindEntry *e = &own->entries[i];
                if (e->dead) continue;
                if (e->serial == mine || (e->serial > mine && !hero_rebind_pinned(e))) hero_rebind_drop(own, i);
            }
        }
        if (lost) own->lost -= 1;
    }
    __sigreturn(context, HERO_REBIND_RETURN_FLAVOR, token);
    __builtin_trap();
}

/* The thunk's question (`parts/sigguard.c`): any entry standing refuses, on
 * whatever stack and however pinned (a handler suspended by a swap is still
 * handling its signal), told as unsure where the innermost is marked so and
 * as switched where it is pinned by a swap or a `setcontext`. No prune by
 * the thunk's own frame (rule 5). */
__attribute__((noinline)) static int hero_rebind_inside(void) {
    HeroSignalStack *own = hero_rebind_own();
    if (own == NULL || !hero_rebind_standing(own)) return HERO_SIG_OUTSIDE;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroRebindEntry *e = &own->entries[i];
        if (e->dead) continue;
        if (e->unsure) {
            return e->unsure == 2   ? HERO_SIG_REENTERED
                   : e->unsure == 3 ? HERO_SIG_ONSTACK
                   : e->unsure == 4 ? HERO_SIG_HELD
                   : e->unsure == 5 ? HERO_SIG_PARKED
                                    : HERO_SIG_UNSURE;
        }
        return hero_rebind_pinned(e) ? HERO_SIG_SWITCHED : HERO_SIG_INSIDE;
    }
    return HERO_SIG_INSIDE;
}

#endif

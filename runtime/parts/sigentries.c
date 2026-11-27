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
 *    outside it, or inside the frames of a handler a jump left that the
 *    thread has not run in since (defect 2855), re-enters a handler's frames
 *    and stands as an entry of its own. A landing at or below a standing
 *    entry on its range is that handler running again.
 * 7. The known stacks are learned where `makecontext` is called (a wrapper in
 *    `sigjump_make.c` on every architecture, since defect d-lane-ctx-after-1:
 *    the C library reads `uc_stack` nowhere else), and nothing tells this
 *    route one has ended: a stale range is never asked about an entry, only
 *    about a target, where it costs a told refusal. Where contexts fall to
 *    route A no switch's target is read, so a range is held from its
 *    `makecontext` on (defect 2745), and a switch to the very context
 *    `makecontext` was handed lands at its range's top.
 * Route A obeys the same rules (defect 2745): its trusted record holds a
 * frame just below the target's stack pointer, on the target's stack, and
 * that bound is judged here as route C's decoded target is, but for whether
 * it lies below the jumping frame, which a bound cannot say and the record's
 * handler serial does (`sigjump_record.c`).
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
 * switch's coroutine. The end is seen by a `swapcontext` into the range that
 * returns while no `swapcontext` or `setcontext` was made FROM the range since
 * it began: a coroutine leaves its range alive only by such a switch (a yield,
 * saving into the context it was switched to or into any other, or a
 * `setcontext` that abandons frames a buffer may still name), and dead only
 * by returning through `uc_link`, which the C library makes unseen. The
 * context switched to is a second witness, copied after the switch by the
 * kernel wherever it is mapped (`hero_jmp_context_moved`): a yield through a
 * switch this file never sees still moves its stack pointer, and a context
 * that cannot be read proves no end (defect 2851). Until defect
 * d-lane-d2745-1 that word was loaded wherever the context lay, and a
 * coroutine that gave its context back crashed the program inside the
 * runtime; and it was the only witness, so a yield saving into another
 * context than the one switched to was judged an end; from then until defect
 * 2851 it was read only where the context provably lived, and a heap
 * context's coroutine yielding unseen was judged ended. A range no switch entered, or whose coroutine ended,
 * holds no coroutine's frames: a save point in it was made by the thread's own
 * code in memory a helper's returned frame once lent `makecontext` (panel
 * 257's `fp3` and the critic's `n6`), and a jump to it is judged on the stack
 * it lies on, the thread's own, where a target above a handler leaves it. A
 * switch into the range clears its end again. A coroutine left by a switch
 * this file never sees that saves into another context than the one it was
 * switched to, leaving that one as it was, is judged ended: said in
 * `sigjump.c`'s module doc.
 *
 * A COROUTINE THE C LIBRARY STARTS THROUGH ANOTHER'S `uc_link` (defect 2850)
 * is entered by no switch this file sees: when the coroutine of a range ends,
 * the C library itself switches to the context its `uc_link` named, which may
 * be one `makecontext` was handed for another range. So the link is learned
 * where `makecontext` reads it (`hero_sig_stack_link`), and a range whose
 * coroutine is entered holds every range down its chain of links entered with
 * it, a range made after the context naming it as well: a coroutine started
 * that way may hold frames from the moment its linker may end. Its end is seen
 * where the swap into the first range of the chain returns: every range down
 * the chain from which no switch was made while the swap waited, and whose
 * context still names the stack pointer it named when the swap began (read
 * by the kernel, `hero_jmp_context_word`), ended, until the first that did
 * not; one whose context cannot be read stays entered. Until this defect such
 * a range was never entered, so a handler's jump into its coroutine, on a
 * stack carved from the thread's own, ran the program's function inside the
 * signal.
 *
 * WHERE CONTEXTS FALL TO ROUTE A (defect 2745) no switch's target is read and
 * no coroutine's end is seen, so a range is HELD from its `makecontext` on:
 * it may hold a coroutine's frames for as long as it stands, and a target in
 * it lies on that coroutine. A switch to the very context `makecontext` was
 * handed for it (`hero_sig_stack_ctx`, a pointer compared, never read)
 * enters it as route C's does; a range held and never so entered is told in
 * words of its own, since no switch into it was seen. */
#define HERO_SIG_RANGE_ENTERED 1
#define HERO_SIG_RANGE_ENDED 2
#define HERO_SIG_RANGE_HELD 4
/* A switch was made from the range while a swap into it waited to return. */
#define HERO_SIG_RANGE_LEFT 8
static _Atomic int hero_sig_stack_state[HERO_SIG_STACKS];
/* How many swaps into a range wait to return: only while one does is a
 * switch asked which range it is made from. */
static _Atomic int hero_sig_watching = 0;
static _Atomic uintptr_t hero_sig_stack_ctx[HERO_SIG_STACKS];
/* The context each range's `uc_link` named when `makecontext` was handed it
 * (0 for none), and the stack pointer each context down a watched chain named
 * when the swap began (0 where it could not be read). */
static _Atomic uintptr_t hero_sig_stack_link[HERO_SIG_STACKS];
static _Atomic uintptr_t hero_sig_stack_start[HERO_SIG_STACKS];

/* `sigjump_record.c`: the stack-pointer word of the context at `context`,
 * read by the kernel wherever it is mapped; false where it cannot be read. */
static bool hero_jmp_context_word(const void *context, uintptr_t *word);

static void hero_sig_stack_put(int i, uintptr_t lo, uintptr_t hi) {
    atomic_fetch_add(&hero_sig_stack_seq[i], 1);
    atomic_store(&hero_sig_stack_lo[i], lo);
    atomic_store(&hero_sig_stack_hi[i], hi);
    atomic_fetch_add(&hero_sig_stack_seq[i], 1);
}

static void hero_sig_range_links(int i);

void hero_sig_known_stack(const void *context, bool held);
void hero_sig_known_stack(const void *context, bool held) {
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
        if (atomic_load(&hero_sig_stack_ctx[i]) == (uintptr_t)context) atomic_store(&hero_sig_stack_ctx[i], 0);
    }
    unsigned at = atomic_fetch_add(&hero_sig_stack_next, 1) % HERO_SIG_STACKS;
    int state = held ? HERO_SIG_RANGE_HELD : 0;
    /* A context another range's `uc_link` names, that range entered: the C
     * library may start this one whenever that coroutine ends. */
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        if ((unsigned)i == at || atomic_load(&hero_sig_stack_link[i]) != (uintptr_t)context) continue;
        int other = atomic_load_explicit(&hero_sig_stack_state[i], memory_order_relaxed);
        if ((other & HERO_SIG_RANGE_ENTERED) != 0 && (other & HERO_SIG_RANGE_ENDED) == 0) {
            state |= HERO_SIG_RANGE_ENTERED;
        }
    }
    atomic_store_explicit(&hero_sig_stack_state[at], state, memory_order_relaxed);
    atomic_store(&hero_sig_stack_ctx[at], (uintptr_t)context);
    atomic_store(&hero_sig_stack_link[at], (uintptr_t)uc->uc_link);
    atomic_store(&hero_sig_stack_start[at], 0);
    hero_sig_stack_put((int)at, lo, hi);
    atomic_store_explicit(&hero_sig_stack_any, 1, memory_order_release);
    atomic_store(&hero_sig_stack_writing, 0);
    if ((state & HERO_SIG_RANGE_ENTERED) != 0) hero_sig_range_links((int)at);
}

/* The slot of the range `makecontext` was handed `context` for, -1 where none
 * stands. */
static int hero_sig_slot_of(uintptr_t context) {
    if (context == 0) return -1;
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        if (atomic_load(&hero_sig_stack_ctx[i]) != context) continue;
        if (atomic_load(&hero_sig_stack_hi[i]) > atomic_load(&hero_sig_stack_lo[i])) return i;
    }
    return -1;
}

/* The ranges down the chain of links from slot `i`, entered with it: the C
 * library starts each when the one before ends. A link already entered and not
 * ended, its own chain was entered when it was. */
static void hero_sig_range_links(int i) {
    for (int step = 0; step < HERO_SIG_STACKS; step += 1) {
        int next = hero_sig_slot_of(atomic_load(&hero_sig_stack_link[i]));
        if (next < 0 || next == i) return;
        int was = atomic_fetch_or_explicit(&hero_sig_stack_state[next], HERO_SIG_RANGE_ENTERED, memory_order_relaxed);
        if ((was & HERO_SIG_RANGE_ENDED) != 0) {
            atomic_fetch_and_explicit(&hero_sig_stack_state[next], ~HERO_SIG_RANGE_ENDED, memory_order_relaxed);
        } else if ((was & HERO_SIG_RANGE_ENTERED) != 0) {
            return;
        }
        i = next;
    }
}

/* Route A's switch to `context`: the range `makecontext` was handed it for,
 * where one was, entered (a pointer compared, nothing of the context read);
 * that range's slot, -1 where none was. */
static int hero_sig_range_switched(const void *context) {
    if (context == NULL || !atomic_load_explicit(&hero_sig_stack_any, memory_order_acquire)) return -1;
    int made = -1;
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        if (atomic_load(&hero_sig_stack_ctx[i]) == (uintptr_t)context) {
            atomic_fetch_or_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_ENTERED, memory_order_relaxed);
            made = i;
        }
    }
    if (made >= 0) hero_sig_range_links(made);
    return made;
}

/* The top of the range in slot `i`, where a context `makecontext` made for it
 * starts (rule 4 judges a stack pointer by the byte below it, which is the
 * range's); false where the slot is being written or names no range now. */
static bool hero_sig_range_top(int i, uintptr_t *t) {
    unsigned before = atomic_load(&hero_sig_stack_seq[i]);
    uintptr_t l = atomic_load(&hero_sig_stack_lo[i]), h = atomic_load(&hero_sig_stack_hi[i]);
    if ((before & 1) != 0 || atomic_load(&hero_sig_stack_seq[i]) != before || h <= l) return false;
    *t = h;
    return true;
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

/* What the known range holding `at` may hold now: 0 no coroutine's frames,
 * `HERO_SIG_RANGE_ENTERED` a coroutine a switch entered and none saw end,
 * `HERO_SIG_RANGE_HELD` a coroutine route A cannot see entered or ended. */
static int hero_sig_range_life(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
    int i = hero_sig_range_slot(at, lo, hi);
    if (i < 0) return 0;
    int state = atomic_load_explicit(&hero_sig_stack_state[i], memory_order_relaxed);
    if ((state & HERO_SIG_RANGE_ENTERED) != 0 && (state & HERO_SIG_RANGE_ENDED) == 0) return HERO_SIG_RANGE_ENTERED;
    return (state & HERO_SIG_RANGE_HELD) != 0 ? HERO_SIG_RANGE_HELD : 0;
}

/* The known range holding `at` that may hold a coroutine's frames now. */
static bool hero_sig_live_range(uintptr_t at, uintptr_t *lo, uintptr_t *hi) { return hero_sig_range_life(at, lo, hi) != 0; }

/* The top of the range that may hold a coroutine's frames now and holds the
 * frame at `frame`: where the frame walk ends (`parts/sigguard.c`, defect
 * 2852). */
static bool hero_sig_coroutine_top(uintptr_t frame, uintptr_t *top) {
    uintptr_t lo = 0;
    return hero_sig_live_range(frame, &lo, top);
}

/* THE KNOWN RANGE A FAULT AT `addr` MAY HAVE RUN OUT OF (defect 2870), for the
 * stack guard's handler (`parts/stack.c`), which asks no other and judges the
 * fault's witnesses itself: of the ranges `makecontext` was handed whose lowest
 * address lies above `addr`, with `sp` below their top, the one whose lowest
 * address lies nearest. Every known range, ended or not: the witnesses say a
 * stack ran out, and a range's life says only where a coroutine's frames may
 * be. Atomic loads alone, a slot read while written skipped. Unused under the
 * sanitizer, whose handler replaces the guard's. */
__attribute__((unused)) static bool hero_sig_range_above(uintptr_t addr, uintptr_t sp, uintptr_t *lo, uintptr_t *hi) {
    if (!atomic_load_explicit(&hero_sig_stack_any, memory_order_acquire)) return false;
    bool found = false;
    for (int i = 0; i < HERO_SIG_STACKS; i += 1) {
        unsigned before = atomic_load(&hero_sig_stack_seq[i]);
        uintptr_t l = atomic_load(&hero_sig_stack_lo[i]), h = atomic_load(&hero_sig_stack_hi[i]);
        if ((before & 1) != 0 || atomic_load(&hero_sig_stack_seq[i]) != before || h <= l) continue;
        if (addr >= l || sp >= h || (found && l >= *lo)) continue;
        *lo = l;
        *hi = h;
        found = true;
    }
    return found;
}

/* A switch whose target's stack pointer is `at` enters the range holding it
 * (a `swapcontext` or `setcontext`, called before anything judges the target). */
static void hero_sig_range_entered(uintptr_t at) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_sig_range_slot(at, &lo, &hi);
    if (i < 0) return;
    atomic_store_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_ENTERED, memory_order_relaxed);
    hero_sig_range_links(i);
}

/* A `swapcontext` or `setcontext` made from a frame at `frame`: while a swap
 * into a range waits to return, the range holding the frame is marked left by
 * a switch, so the swap's return proves no end of its coroutine. */
static void hero_sig_range_left(uintptr_t frame) {
    if (atomic_load_explicit(&hero_sig_watching, memory_order_acquire) == 0) return;
    uintptr_t lo = 0, hi = 0;
    int i = hero_sig_range_slot(frame, &lo, &hi);
    if (i >= 0) atomic_fetch_or_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_LEFT, memory_order_relaxed);
}

/* A `swapcontext` whose target's stack pointer `at` lies on a known range,
 * entered just now, begins to wait: the range's slot and sequence word, -1
 * where no range holds `at` (nothing is then watched). */
static int hero_sig_range_watch(uintptr_t at, unsigned *seq) {
    uintptr_t lo = 0, hi = 0;
    int i = hero_sig_range_slot(at, &lo, &hi);
    if (i < 0) return -1;
    *seq = atomic_load(&hero_sig_stack_seq[i]);
    atomic_fetch_and_explicit(&hero_sig_stack_state[i], ~HERO_SIG_RANGE_LEFT, memory_order_relaxed);
    /* Down its chain of links, what each context names now (defect 2850). */
    int j = i;
    for (int step = 0; step < HERO_SIG_STACKS; step += 1) {
        int next = hero_sig_slot_of(atomic_load(&hero_sig_stack_link[j]));
        if (next < 0 || next == i) break;
        uintptr_t word = 0;
        atomic_fetch_and_explicit(&hero_sig_stack_state[next], ~HERO_SIG_RANGE_LEFT, memory_order_relaxed);
        atomic_store(&hero_sig_stack_start[next],
                     hero_jmp_context_word((const void *)atomic_load(&hero_sig_stack_ctx[next]), &word) ? word : 0);
        j = next;
    }
    atomic_fetch_add_explicit(&hero_sig_watching, 1, memory_order_release);
    return i;
}

/* The swap watching slot `i` returned in its frame at `here` (`ok`: the C
 * library's switch succeeded; `moved`: the context it switched to, copied by
 * the kernel, names another stack pointer since, or cannot be read). Its coroutine ended
 * where the slot still names the range it named, the swap does not run on
 * that range itself, no switch was made from the range since the swap began,
 * and the context did not move: the coroutine returned through `uc_link`. */
static void hero_sig_range_returned(int i, unsigned seq, uintptr_t here, bool ok, bool moved) {
    atomic_fetch_sub_explicit(&hero_sig_watching, 1, memory_order_release);
    if (!ok || moved || atomic_load(&hero_sig_stack_seq[i]) != seq) return;
    uintptr_t lo = atomic_load(&hero_sig_stack_lo[i]), hi = atomic_load(&hero_sig_stack_hi[i]);
    if (atomic_load(&hero_sig_stack_seq[i]) != seq || hero_sig_holds(lo, hi, here)) return;
    int state = atomic_load_explicit(&hero_sig_stack_state[i], memory_order_relaxed);
    if ((state & HERO_SIG_RANGE_LEFT) != 0) return;
    atomic_fetch_or_explicit(&hero_sig_stack_state[i], HERO_SIG_RANGE_ENDED, memory_order_relaxed);
    /* The coroutine ended through its `uc_link`: so did each one down the
     * chain that no switch left and whose context names what it named when the
     * swap began, until the first that did not (defect 2850). */
    int j = i;
    for (int step = 0; step < HERO_SIG_STACKS; step += 1) {
        int next = hero_sig_slot_of(atomic_load(&hero_sig_stack_link[j]));
        if (next < 0 || next == i) return;
        uintptr_t start = atomic_load(&hero_sig_stack_start[next]), word = 0;
        int was = atomic_load_explicit(&hero_sig_stack_state[next], memory_order_relaxed);
        if ((was & HERO_SIG_RANGE_LEFT) != 0 || start == 0) return;
        if (!hero_jmp_context_word((const void *)atomic_load(&hero_sig_stack_ctx[next]), &word) || word != start) return;
        atomic_fetch_or_explicit(&hero_sig_stack_state[next], HERO_SIG_RANGE_ENDED, memory_order_relaxed);
        j = next;
    }
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

/* THE FRAMES OF A HANDLER LEFT BY A JUMP (rule 6, defect 2855). A jump that
 * leaves a handler leaves its frames below its wrapper's frame, down to the
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
static void hero_sig_left_add(HeroSignalStack *own, uintptr_t lo, uintptr_t hi, uintptr_t slo, uintptr_t shi) {
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
static void hero_sig_left_by_jump(HeroSignalStack *own, const HeroSigEntry *e, uintptr_t here) {
    uintptr_t slo = 0, shi = 0;
    if (e->alt_hi > e->alt_lo) {
        slo = e->alt_lo;
        shi = e->alt_hi;
    } else if (!hero_sig_target_range(own, e->at, &slo, &shi)) {
        return;
    }
    uintptr_t lo = hero_sig_holds(slo, shi, here) && here < e->at ? here : slo;
    hero_sig_left_add(own, lo, e->unsure == 2 ? e->at + 1 : e->at, slo, shi);
}

/* The thread seen running at `frame`: every span its stack holds above the
 * frame runs again, from the frame up. */
static void hero_sig_relived(HeroSignalStack *own, uintptr_t frame) {
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
static bool hero_sig_left_holds(const HeroSignalStack *own, uintptr_t t) {
    for (int k = 0; k < own->left_n; k += 1) {
        if (t >= own->left_lo[k] && t < own->left_hi[k]) return true;
    }
    return false;
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
    e->saved = 0;
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
/* A save point made at `frame` (the save hooks and the swap): every entry
 * standing whose own stack holds the frame, below the entry's, has had a save
 * made inside its handler's frames, which can switch back into them however
 * the handler is later left (defect 2872). An entry whose stack is not known
 * is marked too, so it is judged as before. */
static void hero_sig_saved_at(HeroSignalStack *own, uintptr_t frame) {
    if (own == NULL) return;
    for (int i = 0; i < own->depth; i += 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead || frame >= e->at) continue;
        uintptr_t lo = 0, hi = 0;
        if (e->alt_hi > e->alt_lo) {
            lo = e->alt_lo;
            hi = e->alt_hi;
        } else if (!hero_sig_target_range(own, e->at, &lo, &hi)) {
            e->saved = 1;
            continue;
        }
        if (frame >= lo && frame < hi) e->saved = 1;
    }
}

/* Whether a jump from `here` to `t` leaves the handler of `e`, which runs on a
 * coroutine's stack, by leaving that stack (defect 2872): 1 where the jump is
 * made inside the handler's frames there and lands off that stack, no save
 * point having been made inside those frames since the handler began, so
 * nothing can switch back into them but a landing its left span catches
 * (rule 6); 2 where one was, which can; 0 otherwise, the handler on the
 * thread's own stack or an alternate one among them. */
static int hero_sig_off_coroutine(const HeroSigEntry *e, uintptr_t here, uintptr_t t) {
    uintptr_t lo = 0, hi = 0;
    if (e->alt_hi > e->alt_lo || !hero_sig_live_range(e->at, &lo, &hi)) return 0;
    if (here < lo || here >= e->at || (t - 1 >= lo && t - 1 < hi)) return 0;
    return e->saved ? 2 : 1;
}

static void hero_sig_prune_to(HeroSignalStack *own, uintptr_t t, bool by_jump) {
    uintptr_t here = (uintptr_t)__builtin_frame_address(0);
    uintptr_t lo = 0, hi = 0;
    bool ranged = hero_sig_target_range(own, t, &lo, &hi);
    /* A jump onto a coroutine's stack that an entry survives is told as such
     * (`unsure` 3, defect 2705; 4 where route A holds the range and saw no
     * switch into it, defect 2745): the words say what the runtime judged. */
    uintptr_t clo = 0, chi = 0;
    int life = by_jump ? hero_sig_range_life(t, &clo, &chi) : 0;
    bool oncoroutine = life != 0;
    for (int i = own->depth - 1; i >= 0; i -= 1) {
        HeroSigEntry *e = &own->entries[i];
        if (e->dead || hero_sig_pinned(e)) continue;
        bool above = ranged && e->at >= lo && e->at < hi && e->at <= t;
        bool left_alt = by_jump && e->alt_hi > e->alt_lo && !hero_sig_holds(e->alt_lo, e->alt_hi, t);
        int off = by_jump ? hero_sig_off_coroutine(e, here, t) : 0;
        if (above || left_alt || off == 1) {
            hero_sig_left_by_jump(own, e, here);
            hero_sig_drop(own, i);
        } else if (oncoroutine && e->unsure == 0) {
            e->unsure = life == HERO_SIG_RANGE_ENTERED ? 3 : 4;
        } else if (off == 2 && e->unsure == 0) {
            e->unsure = 5;
        }
    }
}

/* Rule 6's last sentence: a jump that lands inside the frames of a handler a
 * jump left, where the thread has not run since (`hero_sig_left_holds`), lands
 * in that handler's frames (shape R: a handler left by a jump out and jumped
 * back into, from its own stack or another), and one that lands on the
 * alternate stack from outside it lands in frames only a signal's delivery
 * puts there. Either stands as an entry of its own, unsure, until a jump above
 * it, the end of a handler pushed before it, or a jump leaving the alternate
 * stack it lies on, proves it left. A landing below the frame making the jump
 * elsewhere is no handler's (defect 2855): a coroutine on a stack carved from
 * the thread's own, entered by a switch the runtime never sees, lands below
 * itself when it leaves for good to its host's save point. */
static void hero_sig_reentered(HeroSignalStack *own, uintptr_t t, bool below, uintptr_t here);

static void hero_sig_reenter(HeroSignalStack *own, uintptr_t t) {
    uintptr_t here = (uintptr_t)__builtin_frame_address(0);
    bool held = hero_sig_left_holds(own, t);
    uintptr_t lo = 0, hi = 0;
    if (!held && own->left_lost) held = hero_sig_target_range(own, here, &lo, &hi) && hero_sig_holds(lo, hi, t) && t < here;
    hero_sig_reentered(own, t, held, here);
}

/* The landing at `t` judged once it is known whether it lies below the frame
 * making the jump (`here`, on the jumping stack): route C knows it by the two
 * addresses; route A, whose `t` is only a bound of the target, by its record
 * naming a handler no longer standing (`sigjump_record.c`). */
static void hero_sig_reentered(HeroSignalStack *own, uintptr_t t, bool below, uintptr_t here) {
    uintptr_t lo = 0, hi = 0, al = 0, ah = 0;
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
        if (e->unsure) {
            return e->unsure == 2   ? HERO_SIG_REENTERED
                   : e->unsure == 3 ? HERO_SIG_ONSTACK
                   : e->unsure == 4 ? HERO_SIG_HELD
                   : e->unsure == 5 ? HERO_SIG_PARKED
                                    : HERO_SIG_UNSURE;
        }
        return hero_sig_pinned(e) ? HERO_SIG_SWITCHED : HERO_SIG_INSIDE;
    }
    return HERO_SIG_INSIDE;
}

#endif

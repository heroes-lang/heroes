/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/stack_lane.c — which stack a frame is on: the thread's own, a stack
 * this runtime knows by its range, or one it cannot name (the defect lane
 * panel258-r1 found beside defect 2752; panel 258's R1 carried to every
 * stack, provisional, author ratification pending; design.md §1.12). Needs
 * `parts/callback.c`'s HeroStackOwn, `parts/stack.c`'s `hero_stack_measure`,
 * and the known ranges of `parts/sigentries.c` (Linux) and
 * `parts/rebind_entries.c` (Darwin).
 *
 * THE FACT. The room a large value takes off its frame (`parts/frame.c`) and
 * the regions a buffer or a local is lent to C from (`parts/lend.c`) are
 * stacks of their own, one each thread, given back in the order of the
 * thread's frames. A coroutine runs on a stack of its own, switched by C
 * (`swapcontext`, a library's own switch, a fiber), and two coroutines on one
 * thread interleave their frames: coroutine 0 takes, yields; coroutine 1
 * takes above it, yields; coroutine 0 resumes, ends, and gives back below
 * coroutine 1's entry. The room said *given back out of order (compiler bug)*
 * (`co_room`, exit 134, 3 runs of 3 on this Mac and 1 of 1 on Linux arm64),
 * the regions *a buffer lent to C was given back at an address its region does
 * not hold*. And where the coroutines' stacks were carved from the thread's
 * own (an array in a frame of the thread, which `makecontext` takes as well as
 * the heap's), R1's healing judged the thread's live entry below a coroutine's
 * frame gone and handed its room to the coroutine's take: a value of the
 * program read another's bytes, 7 printed as 11, 3 runs of 3.
 *
 * SO THE THREAD'S ROOM AND REGIONS HOLD ONLY WHAT FRAMES OF THE THREAD'S OWN
 * STACK TOOK, and an entry taken anywhere else stands ASIDE, a block or a
 * region of its own, given back by its own give in any order and on any
 * thread (`parts/frame_aside.c`, `parts/lend_aside.c`). The thread's own
 * stack is its measured range less every range `makecontext` was given that
 * overlaps it (POSIX), or the stack the thread's block named when the thread
 * first asked (Windows, where a fiber's switch changes the block): a fact of
 * the frame's address, asked at each take, never a premise about how the
 * program switches.
 *
 * AN ENTRY ASIDE IS TAGGED WITH ITS STACK where this runtime can name it: the
 * top of the range `makecontext` was given (Linux arm64 and x86-64, Darwin),
 * or of the stack the thread's block names (Windows). The entries of one tag
 * are of one stack, so R1's healing holds among them; an entry of a stack
 * nobody can name (a library's own switch onto memory outside the thread's
 * stack, glibc's other architectures) is tagged 0 and never judged gone.
 *
 * WHAT IS NOT SEEN, said: a switch onto a stack carved from the thread's own
 * that no `makecontext` this runtime wraps named (a library's own switch, a
 * call of the C library's `makecontext` through a pointer), or one whose
 * range a later 256 others pushed out of the table, is the thread's own stack
 * to every question here, and R1's healing may give back a live entry there. */

#if !defined(_WIN32)
/* The known range holding `at`, and the known ranges overlapping `[lo, hi)`,
 * on the platform that keeps them; none elsewhere. */
static bool hero_lane_known(uintptr_t at, uintptr_t *lo, uintptr_t *hi) {
#if defined(__APPLE__)
    return hero_rebind_known_range(at, lo, hi);
#else
    return hero_sig_known_range(at, lo, hi);
#endif
}

static int hero_lane_carved(uintptr_t lo, uintptr_t hi, uintptr_t *los, uintptr_t *his, int room) {
#if defined(__APPLE__)
    return hero_rebind_ranges_inside(lo, hi, los, his, room);
#else
    return hero_sig_ranges_inside(lo, hi, los, his, room);
#endif
}

/* The ranges carved from the thread's own stack, read again: at most
 * HERO_LANE_CARVED kept, and past that `carved` stays above it so that every
 * frame is asked of the whole table. */
static __attribute__((noinline)) void hero_stack_own_read(HeroStackOwn *own, unsigned made) {
    own->carved = hero_lane_carved(own->lo, own->hi, own->carved_lo, own->carved_hi, HERO_LANE_CARVED);
    own->seen = made;
}
#endif

/* The answer below where the quick one does not hold: the stack not yet
 * measured, a range given since the thread last read them, a frame inside a
 * range carved from the thread's stack or outside the thread's stack. */
static __attribute__((noinline)) int hero_stack_lane_asked(HeroStackOwn *own, uintptr_t at, uintptr_t *stack) {
#if defined(_WIN32)
    const NT_TIB *tib = (const NT_TIB *)NtCurrentTeb();
    uintptr_t base = (uintptr_t)tib->StackBase;
    uintptr_t limit = (uintptr_t)tib->StackLimit;
    if (!own->measured) {
        own->hi = base;
        own->measured = 1;
    }
    int here = at >= limit && at < base;
    if (here && base == own->hi) return 1;
    *stack = here ? base : 0;
    return 0;
#else
    if (!own->measured) {
        hero_stack_measure(&own->lo, &own->hi);
        own->measured = 1;
    }
    uintptr_t lo = 0, hi = 0;
    if (at >= own->lo && at < own->hi) {
        unsigned made = atomic_load_explicit(&hero_stacks_made, memory_order_acquire);
        if (made != own->seen) hero_stack_own_read(own, made);
        if (own->carved == 0) return 1;
        if (own->carved <= HERO_LANE_CARVED) {
            for (int i = 0; i < own->carved; i += 1) {
                if (at >= own->carved_lo[i] && at < own->carved_hi[i]) {
                    *stack = own->carved_hi[i];
                    return 0;
                }
            }
            return 1;
        }
        if (!hero_lane_known(at, &lo, &hi)) return 1;
        *stack = hi;
        return 0;
    }
    *stack = hero_lane_known(at, &lo, &hi) ? hi : 0;
    return 0;
#endif
}

/* Whether `at`, a frame's address, is on the thread's own stack; where it is
 * not, `*stack` names the stack it is on, the top of its range, or 0 where
 * nobody can name it. Inlined into each take, which asks it once: on the
 * thread's own stack, while nothing was carved from it and no range was given
 * since it was last read, one thread-local, one range test and one count
 * compared (Windows asks its thread block each time). */
static inline __attribute__((always_inline)) int hero_stack_lane(uintptr_t at, uintptr_t *stack) {
    HeroStackOwn *own = &hero_stack_own;
#if defined(_WIN32)
    const NT_TIB *tib = (const NT_TIB *)NtCurrentTeb();
    if (own->measured && (uintptr_t)tib->StackBase == own->hi && at >= (uintptr_t)tib->StackLimit && at < own->hi) {
        return 1;
    }
#else
    if (at >= own->lo && at < own->hi && own->carved == 0 &&
        atomic_load_explicit(&hero_stacks_made, memory_order_relaxed) == own->seen) {
        return 1;
    }
#endif
    return hero_stack_lane_asked(own, at, stack);
}

/* THE ASIDES' LOCK. An entry aside may be given back on another thread than
 * took it, a coroutine resumed there, so the asides are the process's, under
 * one lock each (`parts/frame_aside.c`, `parts/lend_aside.c`); a lock is
 * taken only off the thread's own stack. */
#if defined(_WIN32)
typedef SRWLOCK hero_aside_mutex;
#define HERO_ASIDE_LOCK_INIT SRWLOCK_INIT
static void hero_aside_take(hero_aside_mutex *m) { AcquireSRWLockExclusive(m); }
static void hero_aside_drop(hero_aside_mutex *m) { ReleaseSRWLockExclusive(m); }
#else
typedef pthread_mutex_t hero_aside_mutex;
#define HERO_ASIDE_LOCK_INIT PTHREAD_MUTEX_INITIALIZER
static void hero_aside_take(hero_aside_mutex *m) { pthread_mutex_lock(m); }
static void hero_aside_drop(hero_aside_mutex *m) { pthread_mutex_unlock(m); }
#endif

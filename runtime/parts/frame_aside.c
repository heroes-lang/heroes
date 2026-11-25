/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/frame_aside.c — the room of a large value taken on a stack that is not
 * the thread's own: a coroutine's, an alternate stack's, a fiber's (the defect
 * lane panel258-r1 found beside defect 2752; design.md §1.12). Needs
 * `parts/frame.c` and `parts/stack_lane.c`.
 *
 * A BLOCK OF ITS OWN, GIVEN BACK IN ANY ORDER. Frames of two coroutines on one
 * thread interleave, so no one stack's order holds among their takes; each
 * take here is its own block of the raw `malloc` the room's chunks come from,
 * with the header below in front of it, and its give finds that block by its
 * address and hands it back whichever takes stand beside it, on whichever
 * thread resumed the coroutine.
 * It is counted against the room's one gibibyte, so a recursion holding large
 * values on a coroutine's stack stops as it does on the thread's own. Measured
 * on this Mac at `-O2`, a block taken and given back costs 550 instructions
 * more than the room's reused chunk for 32 KB and 1,900 for 256 KB, the
 * value's own bytes written once costing 2,700 and 20,700: the price of
 * stacks whose order nobody can see, paid only off the thread's own stack.
 *
 * AND HEALED ON ITS OWN STACK, where `parts/stack_lane.c` can name that stack:
 * a take, a give and a thunk's entry on a coroutine's stack first give back
 * the blocks of that stack's frames below their own, R1's rule (`parts/
 * callback.c`, WHICH FRAMES A C JUMP LEFT) asked among one stack's blocks
 * alone. A block of a stack nobody can name is never judged gone.
 *
 * WHAT IT DOES NOT DO, said: a coroutine C never resumes leaves its blocks
 * standing until a frame of the same stack is entered again below where they
 * were taken (a thunk at the same address on a stack used again does), and
 * otherwise for the rest of the process; the abandonment is not an event
 * (panel 258's ffi-pragmatist, door 8), and its other values stay leaked as
 * well (panel 258's R2). They count against the gibibyte. */

/* One block's header. `newer` and `older` link every block standing, the
 * newest first, under the lock; `stack` is its stack's tag, 0 for none. */
typedef struct HeroFrameAside {
    struct HeroFrameAside *newer;
    struct HeroFrameAside *older;
    uintptr_t stack;
    uintptr_t frame;
    size_t need;
} HeroFrameAside;

#define HERO_FRAME_ASIDE_HEADER ((sizeof(HeroFrameAside) + 15) & ~(size_t)15)

static hero_aside_mutex hero_frame_aside_lock = HERO_ASIDE_LOCK_INIT;
static HeroFrameAside *hero_frame_aside_newest = NULL;

/* Under the lock: `a` out of the list. */
static void hero_frame_aside_unlink(HeroFrameAside *a) {
    if (a->newer != NULL) a->newer->older = a->older;
    else hero_frame_aside_newest = a->older;
    if (a->older != NULL) a->older->newer = a->newer;
}

/* Under the lock: every block of `stack` whose frame lies below `frame` and
 * that is newer than `stop`, out of the list and onto the returned chain
 * (through `newer`). */
static HeroFrameAside *hero_frame_aside_gone(uintptr_t stack, uintptr_t frame, const HeroFrameAside *stop) {
    HeroFrameAside *gone = NULL;
    HeroFrameAside *a = hero_frame_aside_newest;
    while (a != NULL && a != stop) {
        HeroFrameAside *older = a->older;
        if (a->stack == stack && a->frame < frame) {
            hero_frame_aside_unlink(a);
            a->newer = gone;
            gone = a;
        }
        a = older;
    }
    return gone;
}

/* A block given back, out of every list. */
static void hero_frame_aside_free(HeroFrameAside *a) {
    atomic_fetch_sub(&hero_frame_held, a->need);
    hero_callback_held(-1);
    hero_free_raw(a);
}

static void hero_frame_aside_free_all(HeroFrameAside *gone) {
    while (gone != NULL) {
        HeroFrameAside *next = gone->newer;
        hero_frame_aside_free(gone);
        gone = next;
    }
}

/* Every block of `stack`'s frames below `frame`: those frames are gone, left
 * by a C jump. Nothing for a stack nobody can name. */
static void hero_frame_aside_heal(uintptr_t stack, uintptr_t frame) {
    if (stack == 0) return;
    hero_aside_take(&hero_frame_aside_lock);
    HeroFrameAside *gone = hero_frame_aside_gone(stack, frame, NULL);
    hero_aside_drop(&hero_frame_aside_lock);
    hero_frame_aside_free_all(gone);
}

/* A take by the function whose frame is `frame`, on `stack`; `caller` names
 * it where the room runs out. */
static void *hero_frame_aside_take(size_t bytes, uintptr_t frame, uintptr_t stack, void *caller) {
    hero_frame_aside_heal(stack, frame);
    if (bytes > HERO_FRAME_ROOM) hero_frame_exhausted(caller);
    size_t need = (bytes + 15) & ~(size_t)15;
    size_t before = atomic_fetch_add(&hero_frame_held, need);
    if (before > HERO_FRAME_ROOM - need) {
        atomic_fetch_sub(&hero_frame_held, need);
        hero_frame_exhausted(caller);
    }
    HeroFrameAside *a = (HeroFrameAside *)hero_malloc_raw(HERO_FRAME_ASIDE_HEADER + need);
    char *at = (char *)a + HERO_FRAME_ASIDE_HEADER;
    a->stack = stack;
    a->frame = frame;
    a->need = need;
    a->newer = NULL;
    hero_aside_take(&hero_frame_aside_lock);
    a->older = hero_frame_aside_newest;
    if (a->older != NULL) a->older->newer = a;
    hero_frame_aside_newest = a;
    hero_aside_drop(&hero_frame_aside_lock);
    hero_callback_held(1);
    return at;
}

/* The give of `p`, which no mark of the thread's own room holds: a block
 * aside, found in the list by the address it handed out before a byte of it
 * is read, the blocks newer than it of its stack's frames below its own given
 * back first (its taker is giving it back at its exit, so is the deepest
 * frame alive there); anything else is the compiler's error, told as the
 * room tells one. The search is as long as the blocks standing newer than
 * `p`, every coroutine's, which are as many as their takes held across a
 * switch. */
static void hero_frame_aside_give(void *p) {
    hero_aside_take(&hero_frame_aside_lock);
    HeroFrameAside *a = hero_frame_aside_newest;
    while (a != NULL && (char *)a + HERO_FRAME_ASIDE_HEADER != (char *)p) a = a->older;
    if (a == NULL) {
        hero_aside_drop(&hero_frame_aside_lock);
        hero_frame_out_of_order();
    }
    HeroFrameAside *gone = a->stack != 0 ? hero_frame_aside_gone(a->stack, a->frame, a) : NULL;
    hero_frame_aside_unlink(a);
    hero_aside_drop(&hero_frame_aside_lock);
    hero_frame_aside_free_all(gone);
    hero_frame_aside_free(a);
}

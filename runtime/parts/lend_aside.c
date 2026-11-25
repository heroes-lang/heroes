/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/lend_aside.c — a buffer or a local lent to C from a frame on a stack
 * that is not the thread's own: a coroutine's, an alternate stack's, a fiber's
 * (the defect lane panel258-r1 found beside defect 2752; design.md §1.12,
 * §4.19). Needs `parts/lend.c` and `parts/stack_lane.c`.
 *
 * A REGION OF ITS OWN, GIVEN BACK IN ANY ORDER. The thread's regions are one
 * stack, a depth each, given back in the order of the thread's frames
 * (`parts/lend.c`); frames of two coroutines on one thread interleave, and a
 * local one coroutine lent in place for its function's life stood below the
 * other's at its give, which said *a buffer lent to C was given back at an
 * address its region does not hold*, of a correct program. Here each lend is
 * a region of the same shape, guard page and watches alike, held in a record
 * of its own, and its give finds the record by the address it lent, whichever
 * regions stand beside it and on whichever thread resumed the coroutine.
 *
 * A REGION GIVEN BACK IS WATCHED UNTIL A LEND TAKES IT AGAIN, as a depth's is
 * (`parts/lend.c`'s three watches): summed, or sealed above a mebibyte, or
 * under the sanitizer handed to the thread's quarantine; the next lend aside
 * takes the smallest given back that holds its bytes, auditing it first, and
 * maps one only where none does. So the regions kept are at most as many as
 * were ever lent at once, and their memory as the thread's regions keep it.
 *
 * THE RECORDS ARE IN SLABS NEVER GIVEN BACK, linked from one atomic head, so a
 * record's address holds from its take to its give while other threads lend,
 * and the fault handler, which may run while another thread adds a slab, reads
 * them with no lock and never a slab unmapped (`hero_lend_aside_fault_at`).
 * Everything else here runs under one lock, inside the calling thread's own
 * bracket (`parts/lend.c`'s THE TABLE IS BRACKETED), so a signal handler that
 * lends while its thread is in here stops by name before the lock is asked.
 *
 * AND HEALED ON ITS OWN STACK, as `parts/frame_aside.c` is: a lend, a give and
 * a thunk's entry on a stack `parts/stack_lane.c` names first give back that
 * stack's regions of frames below their own; a stack nobody names is never
 * judged. */

#define HERO_LEND_ASIDE_SLAB 64

/* Free: no memory. Lent: between its take and its give. Given: given back and
 * watched until a lend takes it again. */
#define HERO_LEND_ASIDE_FREE 0
#define HERO_LEND_ASIDE_LENT 1
#define HERO_LEND_ASIDE_GIVEN 2

typedef struct {
    HeroLendRegion r;
    uintptr_t stack;
    int state;
} HeroLendAside;

typedef struct HeroLendSlab {
    struct HeroLendSlab *next;
    HeroLendAside at[HERO_LEND_ASIDE_SLAB];
} HeroLendSlab;

static hero_aside_mutex hero_lend_aside_lock = HERO_ASIDE_LOCK_INIT;
static _Atomic(HeroLendSlab *) hero_lend_slabs = NULL;

/* Under the lock: a slab added, zeroed, its first record handed back. */
static HeroLendAside *hero_lend_aside_slab(void) {
    HeroLendSlab *slab = (HeroLendSlab *)(void *)hero_lend_map(sizeof(HeroLendSlab));
    if (slab == NULL) hero_panic("the system refused the memory for the table of buffers lent to C");
    memset(slab, 0, sizeof(HeroLendSlab));
    slab->next = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed);
    atomic_store_explicit(&hero_lend_slabs, slab, memory_order_release);
    return &slab->at[0];
}

/* Under the lock: a lent region given back and watched, as a depth's is. */
static void hero_lend_aside_retire(HeroLendAside *a) {
    hero_lend_retire(&a->r);
    a->state = a->r.base != NULL ? HERO_LEND_ASIDE_GIVEN : HERO_LEND_ASIDE_FREE;
    hero_callback_held(-1);
}

/* Under the lock: every region lent from `stack`'s frames below `frame`, but
 * `keep`, given back: those frames are gone, left by a C jump. */
static void hero_lend_aside_gone(uintptr_t stack, uintptr_t frame, const HeroLendAside *keep) {
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed); s != NULL; s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            HeroLendAside *a = &s->at[i];
            if (a != keep && a->state == HERO_LEND_ASIDE_LENT && a->stack == stack && a->r.frame < frame) {
                hero_lend_aside_retire(a);
            }
        }
    }
}

/* A lend of the function whose frame is `frame`, on `stack`, a LOCAL's where
 * `local` says so (`parts/lend.c`'s `hero_lend_place`): that stack's regions
 * of frames below it given back first, then the smallest region given back
 * that holds `bytes`, else a record with none, else a new slab's; every field
 * written under the lock, which the other threads' searches read under. */
static HeroLendRegion *hero_lend_aside_push(size_t bytes, const char *callee, const char *param, int64_t extent,
                                            const HeroPlace *where, uintptr_t frame, uintptr_t stack, int local) {
    hero_aside_take(&hero_lend_aside_lock);
    if (stack != 0) hero_lend_aside_gone(stack, frame, NULL);
    HeroLendAside *fits = NULL;
    HeroLendAside *empty = NULL;
    HeroLendAside *given = NULL;
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed); s != NULL; s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            HeroLendAside *a = &s->at[i];
            if (a->state == HERO_LEND_ASIDE_FREE) {
                if (empty == NULL) empty = a;
            } else if (a->state == HERO_LEND_ASIDE_GIVEN) {
                if (a->r.room >= bytes && (fits == NULL || a->r.room < fits->r.room)) fits = a;
                if (given == NULL) given = a;
            }
        }
    }
    HeroLendAside *a = fits != NULL ? fits : empty != NULL ? empty : given != NULL ? given : hero_lend_aside_slab();
    a->state = HERO_LEND_ASIDE_LENT;
    a->stack = stack;
    hero_lend_ready(&a->r, bytes, callee, param, extent, where, frame, "found when the next lend took its memory");
    if (local) hero_lend_label_local(&a->r, callee, param);
    hero_aside_drop(&hero_lend_aside_lock);
    hero_callback_held(1);
    return &a->r;
}

/* The region aside lending `what`, a local's or a buffer's, its stack's
 * regions newer of frames below its own given back first (its lender is
 * giving it back at its exit, so is the deepest frame alive there); NULL where
 * no region aside lends it. The record stays lent until
 * `hero_lend_aside_release`, so no other lend takes it between. */
static HeroLendRegion *hero_lend_aside_find(const char *what, int local) {
    if (atomic_load_explicit(&hero_lend_slabs, memory_order_acquire) == NULL) return NULL;
    hero_aside_take(&hero_lend_aside_lock);
    HeroLendAside *found = NULL;
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed); s != NULL && found == NULL;
         s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            HeroLendAside *a = &s->at[i];
            if (a->state == HERO_LEND_ASIDE_LENT && a->r.local == local && a->r.base + a->r.room - a->r.bytes == what) {
                found = a;
                break;
            }
        }
    }
    if (found != NULL && found->stack != 0) hero_lend_aside_gone(found->stack, found->r.frame, found);
    hero_aside_drop(&hero_lend_aside_lock);
    return found != NULL ? &found->r : NULL;
}

/* The region `hero_lend_aside_find` handed back, given back and watched. */
static void hero_lend_aside_release(HeroLendRegion *r) {
    hero_aside_take(&hero_lend_aside_lock);
    hero_lend_aside_retire((HeroLendAside *)(void *)r);
    hero_aside_drop(&hero_lend_aside_lock);
}

/* `parts/lend.c`'s `hero_lend_local_name` for a local lent aside. */
static void hero_lend_aside_name(const void *local, const char *callee, const char *param) {
    if (atomic_load_explicit(&hero_lend_slabs, memory_order_acquire) == NULL) return;
    hero_aside_take(&hero_lend_aside_lock);
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed); s != NULL; s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            HeroLendRegion *r = &s->at[i].r;
            if (s->at[i].state != HERO_LEND_ASIDE_LENT || !r->local) continue;
            if (r->base + r->room - r->bytes == (const char *)local) {
                r->callee = callee;
                r->param = param;
            }
        }
    }
    hero_aside_drop(&hero_lend_aside_lock);
}

/* The fault handler's question of the regions aside, with no lock and no call
 * (`parts/lend.c`'s `hero_lend_fault_at`): a slab is never given back, so a
 * read here may see a record mid-change and never memory unmapped. */
static int hero_lend_aside_fault_at(uintptr_t addr, HeroLendFault *out) {
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_acquire); s != NULL; s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            const HeroLendAside *a = &s->at[i];
            if (a->state == HERO_LEND_ASIDE_FREE) continue;
            if (hero_lend_fault_in(&a->r, addr, a->state == HERO_LEND_ASIDE_GIVEN, out)) return 1;
        }
    }
    return 0;
}

/* The regions aside given back, audited (`parts/lend.c`'s
 * `hero_lend_audit_given`). */
static void hero_lend_aside_audit(const char *when) {
    if (atomic_load_explicit(&hero_lend_slabs, memory_order_acquire) == NULL) return;
    hero_aside_take(&hero_lend_aside_lock);
    for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_relaxed); s != NULL; s = s->next) {
        for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
            if (s->at[i].state == HERO_LEND_ASIDE_GIVEN) hero_lend_audit(&s->at[i].r, when);
        }
    }
    hero_aside_drop(&hero_lend_aside_lock);
}

/* A thunk entered at `frame` on `stack` (`parts/callback.c`): that stack's
 * regions of frames at or below it given back, in a bracket of its own. */
static void hero_lend_aside_heal(uintptr_t stack, uintptr_t frame) {
    if (stack == 0 || atomic_load_explicit(&hero_lend_slabs, memory_order_acquire) == NULL) return;
    volatile sig_atomic_t *inside = hero_lend_enter();
    hero_aside_take(&hero_lend_aside_lock);
    hero_lend_aside_gone(stack, frame, NULL);
    hero_aside_drop(&hero_lend_aside_lock);
    hero_lend_leave(inside);
}

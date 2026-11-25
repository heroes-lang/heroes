/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/lend_aside.c — the records of the regions a buffer or a local is lent
 * to C from, each region aside from every stack's frames (the defect lane
 * panel258-r1 found beside defect 2752; panel 259's R1, provisional, author
 * ratification pending; design.md §1.12, §4.19). Needs `parts/lend.c` and
 * `parts/frame.c`.
 *
 * A REGION OF ITS OWN, GIVEN BACK IN ANY ORDER. Each lend is a region of
 * `parts/lend.c`'s shape, guard page and watches alike, held in a record of
 * its own, and its give finds the record by the address it lent, whichever
 * regions stand beside it and on whichever thread gives it back: frames of two
 * coroutines on one thread interleave, and one stack of regions a thread gave
 * back in the order of its frames said *a buffer lent to C was given back at
 * an address its region does not hold* of a correct program. NO RECORD IS EVER
 * JUDGED GIVEN BACK BY ITS FRAME'S ADDRESS (panel 259's R1, route E; the head
 * of `parts/frame.c` says why), so a lend C leaves without its give (a jump
 * out of the call it was lent to, or over the callback that lent it) stays
 * lent until the program ends, and its mapped bytes are held against the
 * room's gibibyte from its lend to its give (`hero_frame_room_hold`), so a
 * loop of such leaving ends at that bound's told abort.
 *
 * A REGION GIVEN BACK IS WATCHED UNTIL A LEND TAKES IT AGAIN (`parts/lend.c`'s
 * three watches): summed, or sealed above a mebibyte, or under the sanitizer
 * handed to the giving thread's quarantine; the next lend takes the smallest
 * given back that holds its bytes, auditing it first, and maps one only where
 * none does. So the regions kept are at most as many as were ever lent at
 * once.
 *
 * THE RECORDS ARE IN SLABS NEVER GIVEN BACK, linked from one atomic head, so a
 * record's address holds from its take to its give while other threads lend,
 * and the fault handler, which may run while another thread adds a slab, reads
 * them with no lock and never a slab unmapped (`hero_lend_aside_fault_at`).
 * Everything else here runs under one lock, inside the calling thread's own
 * bracket (`parts/lend.c`'s THE TABLE IS BRACKETED), so a signal handler that
 * lends while its thread is in here stops by name before the lock is asked.
 * What a lend costs here, said: the lock and a walk of every record, about 907
 * instructions more a lend at `-O2` than the thread's own stack of regions
 * before panel 259 (+33%, the synthesis's `cost_lend`). */

#define HERO_LEND_ASIDE_SLAB 64

/* Free: no memory. Lent: between its take and its give. Given: given back and
 * watched until a lend takes it again. */
#define HERO_LEND_ASIDE_FREE 0
#define HERO_LEND_ASIDE_LENT 1
#define HERO_LEND_ASIDE_GIVEN 2

/* `held` is what a lent region holds against the room's bound, its mapped
 * bytes when it was lent, given back with it. */
typedef struct {
    HeroLendRegion r;
    size_t held;
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

/* Under the lock: a lent region given back and watched, its bytes no longer
 * held against the bound. */
static void hero_lend_aside_retire(HeroLendAside *a) {
    hero_frame_room_free(a->held);
    a->held = 0;
    hero_lend_retire(&a->r);
    a->state = a->r.base != NULL ? HERO_LEND_ASIDE_GIVEN : HERO_LEND_ASIDE_FREE;
}

/* A lend of `bytes`, a LOCAL's where `local` says so (`parts/lend.c`'s
 * `hero_lend_local`): the smallest region given back that holds them, else a
 * record with none, else a new slab's; every field written under the lock,
 * which the other threads' searches read under, and the region's mapped bytes
 * held against the bound, `caller` the function named where it stops. */
static HeroLendRegion *hero_lend_aside_push(size_t bytes, const char *callee, const char *param, int64_t extent,
                                            const HeroPlace *where, int local, void *caller) {
    hero_aside_take(&hero_lend_aside_lock);
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
    hero_lend_ready(&a->r, bytes, callee, param, extent, where, "found when the next lend took its memory");
    a->state = HERO_LEND_ASIDE_LENT;
    a->held = a->r.room + a->r.guard;
    hero_frame_room_hold(a->held, caller);
    if (local) hero_lend_label_local(&a->r, callee, param);
    hero_aside_drop(&hero_lend_aside_lock);
    return &a->r;
}

/* The region lending `what`, a local's or a buffer's; NULL where none does.
 * The record stays lent until `hero_lend_aside_release`, so no other lend
 * takes it between. */
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
    hero_aside_drop(&hero_lend_aside_lock);
    return found != NULL ? &found->r : NULL;
}

/* The region `hero_lend_aside_find` handed back, given back and watched. */
static void hero_lend_aside_release(HeroLendRegion *r) {
    hero_aside_take(&hero_lend_aside_lock);
    hero_lend_aside_retire((HeroLendAside *)(void *)r);
    hero_aside_drop(&hero_lend_aside_lock);
}

/* `parts/lend.c`'s `hero_lend_local_name`: the call lending a local now. */
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

/* The fault handler's question of the regions, with no lock and no call
 * (`parts/lend.c`'s `hero_lend_fault_at`): a slab is never given back, so a
 * read here may see a record mid-change and never memory unmapped. A lent
 * region first, wherever it stands in the slabs, then one given back. */
static int hero_lend_aside_fault_at(uintptr_t addr, HeroLendFault *out) {
    for (int lent = 1; lent >= 0; lent -= 1) {
        for (HeroLendSlab *s = atomic_load_explicit(&hero_lend_slabs, memory_order_acquire); s != NULL; s = s->next) {
            for (int i = 0; i < HERO_LEND_ASIDE_SLAB; i += 1) {
                const HeroLendAside *a = &s->at[i];
                if (a->state != (lent ? HERO_LEND_ASIDE_LENT : HERO_LEND_ASIDE_GIVEN)) continue;
                if (hero_lend_fault_in(&a->r, addr, !lent, out)) return 1;
            }
        }
    }
    return 0;
}

/* The regions given back, audited (`parts/lend.c`'s `hero_lend_audit_given`). */
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

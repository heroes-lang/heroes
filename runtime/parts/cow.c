/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/cow.c — copy-on-write: the array primitives that mutate.
 *
 * `xs[i] @ v` mutates a place, and the place may be shared. So: make it unique
 * first, and do it ONCE PER ARRAY STEP of the place path, each writing back at
 * its own level. `**` rather than a returned pointer is what makes the
 * write-back unavoidable — a caller cannot forget to store a result that does
 * not exist.
 *
 * Kept in its own file because the argument for *why* it is per-step is
 * measured, load-bearing and easy to undo by accident; `heroes_runtime.h`
 * carries it in full, and `tests/golden/run/adversarial-cow-per-step.hero` is
 * the program that fails without it while every instrument reports success.
 *
 * design.md §4.10, spec line 60, panel 022.
 */

void hero_array_unshare(HeroArrayHeader **slot) {
    if (slot == NULL) hero_panic("unshare of no slot — a compiler bug");
    HeroArrayHeader *a = *slot;
    hero_array_require(a);
    /* Unique already: mutating in place is unobservable, which is the whole point
     * of copy-on-write. */
    if (a->refcount == 1) return;
    HeroArrayHeader *b = hero_array_new(a->elem, a->len > 0 ? a->len : 1);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        /* SHALLOW plus incref, per element: the copy shares whatever the elements
         * point at, and that sharing is why the *next* step of the place path has
         * to unshare too. Copying deeply here would make the per-step rule
         * unnecessary and pay for every binding what only a mutation costs. */
        a->elem->copy(dst + (size_t)i * size, src + (size_t)i * size);
    }
    b->len = a->len;
    *slot = b;
    hero_array_decref(a);
}

void *hero_array_at_mut(HeroArrayHeader *a, int64_t index) {
    hero_array_require(a);
    if (index < 0 || index >= a->len) hero_panic("array index out of range");
    return hero_array_data(a) + (size_t)index * a->elem->size;
}

/* The place store (heroes_runtime.h carries the contract; panels 037/088).
 * Growth doubles from the existing capacity — Rust, libc++ and Swift double;
 * folly's 1.5 exists to reuse freed memory, which a refcounted block regains
 * on the copying path anyway. Never realloc: the value may point into the
 * old block, and a fresh block keeps it readable until the copies are done. */
void hero_array_push_owned(HeroArrayHeader **slot, const void *value) {
    if (slot == NULL) hero_panic("push into no slot — a compiler bug");
    if (value == NULL) hero_panic("push of no value — a compiler bug");
    HeroArrayHeader *a = *slot;
    hero_array_require(a);
    if (a->len == INT64_MAX) hero_panic("array length overflow");
    if (a->refcount == 1 && a->len < a->cap) {
        unsigned char *dst = hero_array_data(a) + (size_t)a->len * a->elem->size;
        a->elem->copy(dst, value);
        /* The guard the prototypes disagreed on (panel 088 R4 condition 2):
         * the copy may have increfed THIS array. Re-read across it. */
        if (a->refcount == 1) {
            a->len += 1;
            return;
        }
        a->elem->drop(dst);
    }
    int64_t cap = a->cap > 0 ? a->cap : 1;
    while (cap < a->len + 1) {
        if (cap > INT64_MAX / 2) {
            cap = a->len + 1;
            break;
        }
        cap *= 2;
    }
    HeroArrayHeader *b = hero_array_new(a->elem, cap);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *grown = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->copy(grown + (size_t)i * size, src + (size_t)i * size);
    }
    /* Still before the decref: the value may live inside `a`. */
    a->elem->copy(grown + (size_t)a->len * size, value);
    b->len = a->len + 1;
    *slot = b;
    hero_array_decref(a);
}

void hero_array_set(HeroArrayHeader **slot, int64_t index, const void *value) {
    if (slot == NULL) hero_panic("store into no slot — a compiler bug");
    if (value == NULL) hero_panic("store of no value — a compiler bug");
    /* Bounds first, so an out-of-range write aborts without paying for a copy —
     * and so the abort says the same thing whether or not the array was shared. */
    hero_array_require(*slot);
    if (index < 0 || index >= (*slot)->len) hero_panic("array index out of range");
    hero_array_unshare(slot);
    HeroArrayHeader *a = *slot;
    unsigned char *place = hero_array_data(a) + (size_t)index * a->elem->size;
    /* Release what was there, then MOVE the caller's reference in. No copy: the
     * caller increfed before this call, which is what lets the value be something
     * that lived inside the container the unshare just copied. */
    a->elem->drop(place);
    memcpy(place, value, a->elem->size);
}

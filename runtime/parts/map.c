/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/map.c — `{K: V}`: the representation, and everything that reads it.
 *
 * Open addressing with linear probing and THREE PARALLEL REGIONS in one block —
 * a state byte per bucket, then the keys, then the values. Not one struct per
 * bucket, because key and value sizes come from descriptors at runtime: a
 * per-bucket layout would need padding this code computes, while parallel
 * regions need only each region aligned once.
 *
 * The seed is FIXED, and it is not a style choice: the M8c fixpoint compares
 * generated C byte for byte, and a seed that varied per run would make the
 * compiler produce two different correct outputs.
 *
 * Writing is `parts/map-write.c`. The split is by direction rather than by size:
 * everything here is safe on a shared map, and nothing there is.
 *
 * design.md §4.9, §4.20, spec line 58, panels 006, 022 and 026.
 */

/* Every region starts at a multiple of this, so no element is ever misaligned
 * whatever the descriptors say. `max_align_t` is the widest thing C can ask for,
 * which is the only alignment a runtime that does not know its element types can
 * honestly promise. */
#define HERO_MAP_ALIGN (_Alignof(max_align_t))

static size_t hero_map_round_up(size_t n) {
    size_t a = HERO_MAP_ALIGN;
    return (n + a - 1) / a * a;
}

static unsigned char *hero_map_states(HeroMapHeader *m) {
    return (unsigned char *)(void *)m + m->states;
}
static const unsigned char *hero_map_states_const(const HeroMapHeader *m) {
    return (const unsigned char *)(const void *)m + m->states;
}
static unsigned char *hero_map_key_at(HeroMapHeader *m, int64_t at) {
    return (unsigned char *)(void *)m + m->keys + (size_t)at * m->key->size;
}
static const unsigned char *hero_map_key_at_const(const HeroMapHeader *m, int64_t at) {
    return (const unsigned char *)(const void *)m + m->keys + (size_t)at * m->key->size;
}
static unsigned char *hero_map_val_at(HeroMapHeader *m, int64_t at) {
    return (unsigned char *)(void *)m + m->vals + (size_t)at * m->val->size;
}
static const unsigned char *hero_map_val_at_const(const HeroMapHeader *m, int64_t at) {
    return (const unsigned char *)(const void *)m + m->vals + (size_t)at * m->val->size;
}

static void hero_map_require(const HeroMapHeader *m) {
    if (m == NULL) {
        hero_panic("read of an unassigned map slot — this is a compiler bug, please report it");
    }
}

/* THE FIXED SEED. Mixed into every probe so that the bucket a key lands in is a
 * function of the key alone — which is what makes iteration order, and therefore
 * generated C, reproducible across runs (panel 006). */
#define HERO_MAP_SEED UINT64_C(0x9e3779b97f4a7c15)

static int64_t hero_map_slot_of(const HeroMapHeader *m, const void *key) {
    uint64_t h = m->key->hash(key) ^ HERO_MAP_SEED;
    return (int64_t)(h & (uint64_t)(m->cap - 1));
}

HeroMapHeader *hero_map_new(const HeroDesc *key, const HeroDesc *val, int64_t entries) {
    if (key == NULL || val == NULL) hero_panic("map with no descriptor — a compiler bug");
    if (key->hash == NULL) hero_panic("map key descriptor with no hash — a compiler bug");
    if (entries < 0) hero_panic("negative map size");
    /* Load factor at most 1/2, and never zero buckets: linear probing needs a hole
     * to terminate on, and `cap > len` is what guarantees one exists. */
    int64_t cap = 4;
    while (cap < entries * 2 + 1) {
        if (cap > INT64_MAX / 2) hero_panic("map too large");
        cap *= 2;
    }
    size_t head = hero_map_round_up(sizeof(HeroMapHeader));
    size_t states = hero_map_round_up((size_t)cap);
    size_t keys = hero_map_round_up((size_t)cap * key->size);
    size_t vals = (size_t)cap * val->size;
    HeroMapHeader *m = malloc(head + states + keys + vals);
    if (m == NULL) hero_panic("out of memory");
    m->refcount = 1;
    m->len = 0;
    m->cap = cap;
    m->key = key;
    m->val = val;
    m->states = head;
    m->keys = head + states;
    m->vals = head + states + keys;
    memset(hero_map_states(m), 0, (size_t)cap);
    hero_live_blocks += 1;
    return m;
}

void hero_map_incref(HeroMapHeader *m) {
    if (m == NULL) return;
    m->refcount += 1;
}

void hero_map_decref(HeroMapHeader *m) {
    if (m == NULL) return;
    m->refcount -= 1;
    if (m->refcount > 0) return;
    const unsigned char *states = hero_map_states_const(m);
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        m->key->drop(hero_map_key_at(m, i));
        m->val->drop(hero_map_val_at(m, i));
    }
    hero_live_blocks -= 1;
    free(m);
}

int64_t hero_map_len(const HeroMapHeader *m) {
    hero_map_require(m);
    return m->len;
}

void hero_map_put(HeroMapHeader *m, const void *key, const void *value) {
    hero_map_require(m);
    unsigned char *states = hero_map_states(m);
    int64_t at = hero_map_slot_of(m, key);
    for (int64_t probe = 0; probe < m->cap; probe++) {
        int64_t i = (at + probe) & (m->cap - 1);
        if (states[i] == 0) {
            /* COPY, not move — the same rule as `hero_array_push`, and for the same
             * reason: every value reaching a constructor is borrowed (the ownership
             * pass's rule 5), so the container has to take its own reference. */
            m->key->copy(hero_map_key_at(m, i), key);
            m->val->copy(hero_map_val_at(m, i), value);
            states[i] = 1;
            m->len += 1;
            return;
        }
        if (m->key->eq(hero_map_key_at_const(m, i), key)) {
            /* A duplicate key REPLACES: `{"a": 1, "a": 2}` has to mean something, and
             * the later entry is what a reader of the literal expects. The key that
             * later entry is what a reader of the literal expects. Only the VALUE is
             * replaced: the keys are equal, so which of the two copies the map keeps is
             * unobservable, and keeping the first saves a write and a reference. */
            m->val->drop(hero_map_val_at(m, i));
            m->val->copy(hero_map_val_at(m, i), value);
            return;
        }
    }
    /* `cap > 2 * len` is maintained by `hero_map_new`, so a literal cannot fill its
     * own table. Reaching here means the invariant broke. */
    hero_panic("map is full — this is a compiler bug, please report it");
}

const void *hero_map_find(const HeroMapHeader *m, const void *key) {
    hero_map_require(m);
    const unsigned char *states = hero_map_states_const(m);
    int64_t at = hero_map_slot_of(m, key);
    for (int64_t probe = 0; probe < m->cap; probe++) {
        int64_t i = (at + probe) & (m->cap - 1);
        /* An empty bucket ends the probe: with no deletion there are no tombstones,
         * so a hole means the key was never inserted. */
        if (states[i] == 0) return NULL;
        if (m->key->eq(hero_map_key_at_const(m, i), key)) {
            return hero_map_val_at_const(m, i);
        }
    }
    return NULL;
}

bool hero_map_eq(const HeroMapHeader *a, const HeroMapHeader *b) {
    hero_map_require(a);
    hero_map_require(b);
    if (a->len != b->len) return false;
    if (a == b) return true;
    const unsigned char *states = hero_map_states_const(a);
    for (int64_t i = 0; i < a->cap; i++) {
        if (states[i] == 0) continue;
        const void *found = hero_map_find(b, hero_map_key_at_const(a, i));
        if (found == NULL) return false;
        if (!a->val->eq(hero_map_val_at_const(a, i), found)) return false;
    }
    return true;
}

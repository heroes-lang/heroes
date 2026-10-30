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
 * The seed is FIXED, and it is not a style choice: the M-selfhost-fixpoint fixpoint compares
 * generated C byte for byte, and a seed that varied per run would make the
 * compiler produce two different correct outputs.
 *
 * **And a FOURTH region since 2026-10-29: the order the keys were first
 * written in** (feature 2274; panel 232's R12, which the author ratified with
 * insertion order). `keys` reads the keys in that order, and growth and the
 * copy-on-write rebuild re-insert in it, so no table built from another loses
 * it; a key written again keeps its place, as a Python dict's does since 3.7.
 * Lookups never read it, so a probe costs what it did.
 *
 * Writing is `parts/map-write.c`. The split is by direction rather than by size:
 * everything here is safe on a shared map, and nothing there is.
 *
 * design.md §4.9, §4.20, spec § 7 Operators, panels 006, 022 and 026.
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

/* THE ORDER REGION: for each live entry, in the order it was first inserted,
 * the bucket it sits in. A bucket never moves inside one table (there is no
 * deletion, so no tombstone and no compaction), which is what lets a bucket's
 * index stand for an entry. `cap / 2` slots, the most entries a table holds:
 * `hero_map_set_at` grows before a write would pass half its buckets, and
 * `hero_map_new` gives a literal of `entries` keys more than twice as many.
 * Its offset is computed rather than stored, after the values' region rounded
 * as every region is, so `HeroMapHeader` and the ABI keep their shape. */
static size_t hero_map_order_offset(const HeroMapHeader *m) {
    return m->vals + hero_map_round_up((size_t)m->cap * m->val->size);
}
static int64_t *hero_map_order(HeroMapHeader *m) {
    return (int64_t *)(void *)((unsigned char *)(void *)m + hero_map_order_offset(m));
}

/* A new entry's place in the order, written before `len` counts it. */
static void hero_map_order_append(HeroMapHeader *m, int64_t bucket) {
    if (m->len >= m->cap / 2) hero_panic("map order is full — this is a compiler bug, please report it");
    hero_map_order(m)[m->len] = bucket;
}

/* The bucket of the entry written `at`-th, `0 <= at < len`. Checked where it
 * is read, as `keys` checks its count (2026-08-12, sweep 001 S10): an order
 * naming an empty bucket would copy what is not there. */
static int64_t hero_map_entry_bucket(const HeroMapHeader *m, int64_t at) {
    const unsigned char *base = (const unsigned char *)(const void *)m;
    int64_t i = ((const int64_t *)(const void *)(base + hero_map_order_offset(m)))[at];
    if (i < 0 || i >= m->cap || hero_map_states_const(m)[i] == 0) {
        hero_panic("map order disagrees with its buckets — this is a compiler bug, please report it");
    }
    return i;
}

/* Every reader goes through this: NULL is an unassigned slot. **And a map
 * whose block was given back** (defect 462's map, 2026-10-07): the key
 * descriptor, which a live map always has, is set to none before the block
 * goes (defect 314, below), and a read asked nothing of it, so a map made for
 * four entries, released and then asked its length, answered from the freed
 * block at exit 0, measured on this Mac at `b1784b65`. One load and one test a
 * read, inlined, `hero_array_require`'s. */
_Noreturn static void hero_map_read_released(void) {
    hero_panic("a map read after its last reference was released — a compiler bug, please report it");
}

__attribute__((always_inline)) static inline void hero_map_require(const HeroMapHeader *m) {
    if (m == NULL) {
        hero_panic("read of an unassigned map slot — this is a compiler bug, please report it");
    }
    if (m->key == NULL) hero_map_read_released();
}

/* THE FIXED SEED. Mixed into every probe so that the bucket a key lands in is a
 * function of the key alone — which made iteration order, and therefore
 * generated C, reproducible across runs (panel 006). Since feature 2274 no
 * reader shows the buckets' order, the keys coming in the order they were
 * written; the seed still makes every run's probes, and so its work, the same. */
#define HERO_MAP_SEED UINT64_C(0x9e3779b97f4a7c15)

/* A KEY MUST EQUAL ITSELF, and it is checked here because every probe starts
 * here — insert, lookup and the growth rebuild all call this before touching a
 * bucket.
 *
 * The premise the probe rests on is reflexivity, not the `eq ⇒ same hash` rule
 * the `-0.0` normalisation in `desc.c` defends. Nothing enforced it: `{K: V}`
 * accepts any key type, and `0.0 / 0.0` is a legal Heroes expression, so a NaN
 * key inserted twice made `len` 2, the lookup that follows returned
 * `missing_key`, and `{nan: 1} == {nan: 1}` was false — at exit 0, ASan clean,
 * leak counter balanced. Every instrument this project owns reported success,
 * which is the class §1.2 exists to eliminate (2026-08-12, sweep 001 audit S1).
 *
 * It aborts rather than inventing an order, exactly as `sort` does one file
 * over, and for the reason written there: a wrong answer with no error is what
 * the M-selfhost-fixpoint fixpoint cannot have. */
static int64_t hero_map_slot_where(const HeroMapHeader *m, const void *key, const HeroPlace *where) {
    if (!m->key->eq(key, key)) hero_panic_where("a map key that is not equal to itself (nan)", where);
    uint64_t h = m->key->hash(key) ^ HERO_MAP_SEED;
    return (int64_t)(h & (uint64_t)(m->cap - 1));
}

HeroMapHeader *hero_map_new(const HeroDesc *key, const HeroDesc *val, int64_t entries) {
    if (key == NULL || val == NULL) hero_panic("map with no descriptor — a compiler bug");
    if (key->hash == NULL) hero_panic("map key descriptor with no hash — a compiler bug");
    if (key->eq == NULL) hero_panic("map key descriptor with no eq — a compiler bug");
    /* The VALUE's too, and symmetrically: `hero_map_hash` calls `val->hash` and
     * `hero_map_eq` calls `val->eq`, so a null there is the same crash with no
     * type name in it that panel 022 wrote the key's guard for. One side was
     * checked and the other was not (2026-08-12, sweep 001 audit L3). */
    if (val->hash == NULL) hero_panic("map value descriptor with no hash — a compiler bug");
    if (val->eq == NULL) hero_panic("map value descriptor with no eq — a compiler bug");
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
    size_t vals = hero_map_round_up((size_t)cap * val->size);
    size_t order = (size_t)(cap / 2) * sizeof(int64_t);
    HeroMapHeader *m = hero_alloc_block(head + states + keys + vals + order);
    if (m == NULL) hero_panic("out of memory");
    /* Relaxed on a block no other thread can see yet — `str.c`'s reason. */
    atomic_store_explicit(&m->refcount, 1, memory_order_relaxed);
    m->len = 0;
    m->cap = cap;
    m->key = key;
    m->val = val;
    m->states = head;
    m->keys = head + states;
    m->vals = head + states + keys;
    memset(hero_map_states(m), 0, (size_t)cap);
    return m;
}

/* **A map released after its last reference is refused by name**, as an
 * array is (defect 314, 2026-10-07): until then a second release here freed
 * the block again, taking a count of 0 to -1 in memory already given back,
 * which this Mac's allocator answered with its own trap at 4 and 32 entries
 * (exit 133, no word of the runtime's) and passed at 1 (exit 0), and the
 * Windows heap stopped as heap corruption with nothing said. The count is
 * asked first, and the key descriptor, which a live map always has
 * (`hero_map_new` refuses none), is set to none before the block goes, so a
 * release that finds the allocator's word where the count was is still
 * refused. There is no constant map, so no count of -1 to pass. */
_Noreturn static void hero_map_overreleased(void) {
    hero_panic("a map released after its last reference — a compiler bug, please report it");
}

/* The same two orders as `str.c` and `array.c`, for the same reason. */
void hero_map_incref(HeroMapHeader *m) {
    if (m == NULL) return;
    if (atomic_load_explicit(&m->refcount, memory_order_relaxed) < 1 || m->key == NULL) hero_map_overreleased();
    atomic_fetch_add_explicit(&m->refcount, 1, memory_order_relaxed);
}

/* `hero_array_decref`'s rule at the other container, and it must be the same
 * rule: a value may alternate arrays and maps, so a map that released its
 * entries recursively would put the frames back that `drop.c` exists to remove. */
void hero_map_decref(HeroMapHeader *m) {
    if (m == NULL) return;
    if (atomic_load_explicit(&m->refcount, memory_order_relaxed) < 1 || m->key == NULL) hero_map_overreleased();
    int64_t before = atomic_fetch_sub_explicit(&m->refcount, 1, memory_order_acq_rel);
    if (before > 1) return;
    if (before < 1) hero_map_overreleased();
    if (hero_drop_running) {
        hero_drop_push_map(m);
        return;
    }
    hero_drop_running = true;
    hero_drop_drain_from_map(m);
    hero_drop_running = false;
}

/* The exit's release through the slot (defect 470). */
void hero_map_release_at(HeroMapHeader *const *slot) { hero_map_decref(*slot); }

/* `hero_array_release_contents`' counterpart — see `drop.c`. */
static void hero_map_release_contents(HeroMapHeader *m) {
    const unsigned char *states = hero_map_states_const(m);
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        m->key->drop(hero_map_key_at(m, i));
        m->val->drop(hero_map_val_at(m, i));
    }
    m->key = NULL;
    hero_release_block(m);
}

int64_t hero_map_len(const HeroMapHeader *m) {
    hero_map_require(m);
    return m->len;
}

/* The three that reach a key's slot hold their bodies in the `_at` twins, which
 * take the place its refusal names (panel 230's R1, kind 2): the site's record
 * from the program's C, NULL from the old entry points and from the runtime's
 * own re-inserts and lookups, whose keys stand in a map already. */
void hero_map_put_at(HeroMapHeader *m, const void *key, const void *value, const HeroPlace *where) {
    hero_map_require(m);
    unsigned char *states = hero_map_states(m);
    int64_t at = hero_map_slot_where(m, key, where);
    for (int64_t probe = 0; probe < m->cap; probe++) {
        int64_t i = (at + probe) & (m->cap - 1);
        if (states[i] == 0) {
            /* COPY, not move — the same rule as `hero_array_push`, and for the same
             * reason: every value reaching a constructor is borrowed (the ownership
             * pass's rule 5), so the container has to take its own reference. */
            m->key->copy(hero_map_key_at(m, i), key);
            m->val->copy(hero_map_val_at(m, i), value);
            states[i] = 1;
            hero_map_order_append(m, i);
            m->len += 1;
            return;
        }
        if (m->key->eq(hero_map_key_at_const(m, i), key)) {
            /* A duplicate key REPLACES: `{"a": 1, "a": 2}` has to mean something, and
             * the later entry is what a reader of the literal expects. Only the VALUE is
             * replaced: the keys are equal, so which of the two copies the map keeps is
             * unobservable, and keeping the first saves a write and a reference. The
             * key keeps the place of its first writing in the order. */
            m->val->drop(hero_map_val_at(m, i));
            m->val->copy(hero_map_val_at(m, i), value);
            return;
        }
    }
    /* `cap > 2 * len` is maintained by `hero_map_new`, so a literal cannot fill its
     * own table. Reaching here means the invariant broke. */
    hero_panic("map is full — this is a compiler bug, please report it");
}

void hero_map_put(HeroMapHeader *m, const void *key, const void *value) { hero_map_put_at(m, key, value, NULL); }

const void *hero_map_find_at(const HeroMapHeader *m, const void *key, const HeroPlace *where) {
    hero_map_require(m);
    const unsigned char *states = hero_map_states_const(m);
    int64_t at = hero_map_slot_where(m, key, where);
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

const void *hero_map_find(const HeroMapHeader *m, const void *key) { return hero_map_find_at(m, key, NULL); }

/* Contents and nothing else — `hero_array_eq`'s rule, deleted here for the same
 * reason on the same day (panel 069).
 *
 * This copy was the one the sitting's brief missed: a nan KEY aborts at
 * `hero_map_slot_of` and cannot reach here, so the site looked unreachable. It is
 * reachable through a nan VALUE — `{str: f64}` with `b = a` answered true and a
 * rebuilt twin answered false. A defence that rests on "the other guard catches
 * it" is a premise about a neighbouring file, and this one was already wrong. */
bool hero_map_eq(const HeroMapHeader *a, const HeroMapHeader *b) {
    hero_map_require(a);
    hero_map_require(b);
    if (a->len != b->len) return false;
    const unsigned char *states = hero_map_states_const(a);
    for (int64_t i = 0; i < a->cap; i++) {
        if (states[i] == 0) continue;
        const void *found = hero_map_find_at(b, hero_map_key_at_const(a, i), NULL);
        if (found == NULL) return false;
        if (!a->val->eq(hero_map_val_at_const(a, i), found)) return false;
    }
    return true;
}

/* parts/map-write.c — the map's descriptor, `keys`, and everything that writes.
 *
 * Growth, the copy-on-write the map shipped without for one commit, and the
 * place store behind `m[k] @ v`. Two rules govern all of it and both were
 * learned by a defect:
 *
 *   - every copy goes through `hero_map_put` and every release through
 *     `hero_map_decref`, so the refcount arithmetic lives in exactly one place;
 *   - the key is COPIED and the value is MOVED, because `own.rs` increfs before
 *     an indexed store precisely so the primitive does not have to.
 *
 * Needs `parts/array.c` for `keys`, which hands back a fresh `[K]`.
 *
 * design.md §4.9, §4.20, spec lines 60 and 70, panels 022 and 026.
 */

/* One descriptor for every `{K: V}`, for the same reason as the array's: copy, drop
 * and eq on a map value reach the key and value types through the header. */
static void hero_copy_map(void *dst, const void *src) {
    HeroMapHeader *m = *(HeroMapHeader *const *)src;
    hero_map_incref(m);
    *(HeroMapHeader **)dst = m;
}
static void hero_drop_map(void *elem) { hero_map_decref(*(HeroMapHeader **)elem); }
static bool hero_eq_map(const void *a, const void *b) {
    return hero_map_eq(*(HeroMapHeader *const *)a, *(HeroMapHeader *const *)b);
}
/* Order-INDEPENDENT, and it has to be: `eq` ignores insertion order, so a hash that
 * did not would give two equal maps two hashes. XOR of the per-entry mixes is
 * commutative, which is the cheapest way to say that. */
static uint64_t hero_hash_map(const void *elem) {
    const HeroMapHeader *m = *(const HeroMapHeader *const *)elem;
    hero_map_require(m);
    const unsigned char *states = hero_map_states_const(m);
    uint64_t h = 0;
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        uint64_t k = m->key->hash(hero_map_key_at_const(m, i));
        uint64_t v = m->val->hash(hero_map_val_at_const(m, i));
        h ^= (k ^ (v * UINT64_C(0x100000001b3)));
    }
    return h ^ (uint64_t)m->len;
}

const HeroDesc hero_desc_map = {sizeof(HeroMapHeader *), hero_copy_map, hero_drop_map,
                                hero_eq_map, hero_hash_map};

HeroArrayHeader *hero_map_keys(const HeroMapHeader *m) {
    hero_map_require(m);
    HeroArrayHeader *out = hero_array_new(m->key, m->len > 0 ? m->len : 1);
    const unsigned char *states = hero_map_states_const(m);
    unsigned char *data = (unsigned char *)(void *)(out + 1);
    int64_t at = 0;
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        /* Copied, not moved: the map keeps its own key and the array takes one. */
        m->key->copy(data + (size_t)at * m->key->size, hero_map_key_at_const(m, i));
        at += 1;
    }
    out->len = at;
    return out;
}

/* Grow and re-probe, through the PUBLIC copy path.
 *
 * Written first as `memcpy` of the live entries plus `free` of the old block
 * without dropping — the reasoning being that the references pass to the new block
 * unchanged, so no descriptor should run. That reasoning is right about the
 * references and wrong about everything else: it hand-rolls refcount bookkeeping in
 * a second place, and the first program to grow a map and then read its keys
 * panicked with `not a Heroes string block`, every printed answer correct and the
 * failure at exit.
 *
 * This version copies through `hero_map_put` (+1 per entry, via the descriptors)
 * and releases the old map through `hero_map_decref` (−1 per entry, via the same
 * descriptors). Balanced by construction, with no arithmetic of mine in it. A
 * growth costs one extra copy of every entry, and a rehash was already paying for
 * one. */
static HeroMapHeader *hero_map_grown(HeroMapHeader *m) {
    HeroMapHeader *b = hero_map_new(m->key, m->val, m->len * 2 + 1);
    const unsigned char *states = hero_map_states_const(m);
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        hero_map_put(b, hero_map_key_at_const(m, i), hero_map_val_at_const(m, i));
    }
    hero_map_decref(m);
    return b;
}

/* THE COPY-ON-WRITE THE MAP SHIPPED WITHOUT (M6 step 3).
 *
 * `hero_array_unshare`'s twin, and it was missing for one commit: M6 step 2 wrote
 * `hero_map_set` to write in place with no refcount check, so `n = m` followed by
 * `m["b"] @ 2` changed `n` as well — `2 2 2` where spec line 60 requires `1 2 -1`.
 * ASan clean, leak counter zero, exit 0: a green harness on a program that
 * violates "no aliasing exists anywhere", which is exactly the shape panel 022
 * measured for arrays and exactly the container that landed after it.
 *
 * The copy goes through `hero_map_put`, which increfs every key and value through
 * their descriptors — the same balanced-by-construction rule `hero_map_grown`
 * learned the hard way. There is no per-step question here as there is for
 * arrays: a map is only ever the LAST step of a place path (`emit/aggregate.rs`
 * refuses a non-final map step), because there is no element to descend into. */
static void hero_map_unshare(HeroMapHeader **slot) {
    HeroMapHeader *m = *slot;
    if (m->refcount == 1) return;
    HeroMapHeader *copy = hero_map_new(m->key, m->val, m->len);
    const unsigned char *states = hero_map_states_const(m);
    for (int64_t i = 0; i < m->cap; i++) {
        if (states[i] == 0) continue;
        hero_map_put(copy, hero_map_key_at_const(m, i), hero_map_val_at_const(m, i));
    }
    hero_map_decref(m);
    *slot = copy;
}

void hero_map_set(HeroMapHeader **slot, const void *key, const void *value) {
    if (slot == NULL) hero_panic("store into no map — a compiler bug");
    if (value == NULL) hero_panic("store of no value — a compiler bug");
    hero_map_require(*slot);
    hero_map_unshare(slot);
    HeroMapHeader *m = *slot;
    unsigned char *states = hero_map_states(m);
    int64_t at = hero_map_slot_of(m, key);
    for (int64_t probe = 0; probe < m->cap; probe++) {
        int64_t i = (at + probe) & (m->cap - 1);
        if (states[i] == 0) {
            /* Load factor at most 1/2, the invariant `hero_map_new` establishes and
             * this is the only place that can break it. Grow first, then retry from
             * the top: the bucket for this key is different in the new table. */
            if ((m->len + 1) * 2 > m->cap) {
                *slot = hero_map_grown(m);
                hero_map_set(slot, key, value);
                return;
            }
            /* The key is COPIED (the caller lends it) and the value is MOVED —
             * `hero_array_set`'s rule, and the ownership pass's: `own.rs` increfs
             * before an indexed store precisely so the primitive does not have to.
             * Copying here instead increfs a second time, which is the leak M6
             * step 2 shipped: `m["a"] @ "x" + "y"` printed `xy` and then
             * `1 heap blocks still live at exit`. */
            m->key->copy(hero_map_key_at(m, i), key);
            memcpy(hero_map_val_at(m, i), value, m->val->size);
            states[i] = 1;
            m->len += 1;
            return;
        }
        if (m->key->eq(hero_map_key_at_const(m, i), key)) {
            /* Replace the value in place and keep the key that is there — they are
             * equal, so which copy the map holds is unobservable. Release what was
             * there, then move the caller's reference in. */
            m->val->drop(hero_map_val_at(m, i));
            memcpy(hero_map_val_at(m, i), value, m->val->size);
            return;
        }
    }
    hero_panic("map is full — this is a compiler bug, please report it");
}

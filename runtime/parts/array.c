/* SPDX-License-Identifier: Apache-2.0, with the Heroes runtime exception:
 * see LICENSE-RUNTIME-EXCEPTION. A program compiled with Heroes carries
 * part of this runtime inside it and owes nothing for doing so. */
/* parts/array.c — `[T]`, and the operations that do not mutate.
 *
 * A pointer to a heap header followed by the elements IN-LINE. The array is
 * Heroes' only indirection (§4.10), which is what gives a recursive type a
 * finite size, and being one pointer wide is why an array field imposes no
 * ordering constraint on the generated C.
 *
 * Everything here works through the element descriptor, so nothing in this file
 * knows what the array holds. The mutating half is `parts/cow.c`, kept apart
 * because copy-on-write is the subtlest rule in this runtime.
 *
 * design.md §4.10, §4.20, spike 04's frozen ABI.
 */

/* Declared here, defined after the array functions they call. */
static void hero_copy_array(void *dst, const void *src);
static void hero_drop_array(void *elem);
static bool hero_eq_array(const void *a, const void *b);
static uint64_t hero_hash_array(const void *elem);

const HeroDesc hero_desc_array = {sizeof(HeroArrayHeader *), hero_copy_array,
                                  hero_drop_array, hero_eq_array, hero_hash_array};

static unsigned char *hero_array_data(HeroArrayHeader *a) {
    return (unsigned char *)(void *)(a + 1);
}
static const unsigned char *hero_array_data_const(const HeroArrayHeader *a) {
    return (const unsigned char *)(const void *)(a + 1);
}

/* Every reader goes through this: NULL is an unassigned or moved-out slot, and
 * reading one is a compiler bug, not an empty array. */
static void hero_array_require(const HeroArrayHeader *a) {
    if (a == NULL) {
        hero_panic("read of an unassigned array slot — this is a compiler bug, please report it");
    }
}

HeroArrayHeader *hero_array_new(const HeroDesc *elem, int64_t cap) {
    if (elem == NULL) hero_panic("array with no element descriptor — a compiler bug");
    if (elem->hash == NULL) hero_panic("element descriptor with no hash — a compiler bug");
    if (cap < 1) cap = 1;
    if ((uint64_t)cap > (SIZE_MAX - sizeof(HeroArrayHeader)) / elem->size) {
        hero_panic("array too large");
    }
    HeroArrayHeader *a = malloc(sizeof(HeroArrayHeader) + (size_t)cap * elem->size);
    if (a == NULL) hero_panic("out of memory");
    a->refcount = 1;
    a->len = 0;
    a->cap = cap;
    a->elem = elem;
    hero_live_blocks += 1;
    return a;
}

void hero_array_incref(HeroArrayHeader *a) {
    if (a == NULL) return;
    a->refcount += 1;
}

void hero_array_decref(HeroArrayHeader *a) {
    if (a == NULL) return; /* the zero-init non-value: a no-op */
    a->refcount -= 1;
    if (a->refcount > 0) return;
    unsigned char *data = hero_array_data(a);
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->drop(data + (size_t)i * a->elem->size);
    }
    hero_live_blocks -= 1;
    free(a);
}

int64_t hero_array_len(const HeroArrayHeader *a) {
    hero_array_require(a);
    return a->len;
}

const void *hero_array_at(const HeroArrayHeader *a, int64_t index) {
    hero_array_require(a);
    if (index < 0 || index >= a->len) hero_panic("array index out of range");
    return hero_array_data_const(a) + (size_t)index * a->elem->size;
}

HeroArrayHeader *hero_array_push(const HeroArrayHeader *a, const void *elem) {
    hero_array_require(a);
    if (a->len == INT64_MAX) hero_panic("array length overflow");
    HeroArrayHeader *b = hero_array_new(a->elem, a->len + 1);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)i * size);
    }
    /* The new element is copied in, not moved: the caller's value is borrowed
     * (every value reaching a call is, by the ownership pass's rule 5), so the
     * array takes its own reference exactly as it does for the ones it copied. */
    a->elem->copy(dst + (size_t)a->len * size, elem);
    b->len = a->len + 1;
    return b;
}

bool hero_array_eq(const HeroArrayHeader *a, const HeroArrayHeader *b) {
    hero_array_require(a);
    hero_array_require(b);
    if (a->len != b->len) return false;
    if (a == b) return true;
    const unsigned char *da = hero_array_data_const(a);
    const unsigned char *db = hero_array_data_const(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        if (!a->elem->eq(da + (size_t)i * size, db + (size_t)i * size)) return false;
    }
    return true;
}

/* `slice(xs, from:, to:)` on an array — a NEW array, elements copied through the
 * descriptor, and it ABORTS out of range with the same three-part test and the
 * same shape of message as `hero_str_slice`.
 *
 * Abort rather than clamp (panel 027 R3): a clamping slice hands a shorter array
 * to whatever comes next, and when that next thing is a length passed to C —
 * §1.11's whole point — the mismatch is silent. Go, Rust's indexing form and Zig
 * all abort; Python clamps, and nobody has ever documented what that cost. */
HeroArrayHeader *hero_array_slice(const HeroArrayHeader *a, int64_t from, int64_t to) {
    hero_array_require(a);
    if (from < 0 || to < from || to > a->len) hero_panic("array slice out of range");
    int64_t n = to - from;
    HeroArrayHeader *b = hero_array_new(a->elem, n);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < n; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)(from + i) * size);
    }
    b->len = n;
    return b;
}

/* One descriptor for every `[T]`: these four reach the element type through the
 * header, so nothing here depends on what the array holds. */
static void hero_copy_array(void *dst, const void *src) {
    HeroArrayHeader *a = *(HeroArrayHeader *const *)src;
    hero_array_incref(a);
    *(HeroArrayHeader **)dst = a;
}
static void hero_drop_array(void *elem) { hero_array_decref(*(HeroArrayHeader **)elem); }
static bool hero_eq_array(const void *a, const void *b) {
    return hero_array_eq(*(HeroArrayHeader *const *)a, *(HeroArrayHeader *const *)b);
}
/* An array is hashable so that a descriptor's `hash` is never null (panel 022),
 * not because an array can be a map key — `{[int]: v}` is a question §4.9 has
 * not answered. Order matters, because `==` on an array is order-sensitive. */
static uint64_t hero_hash_array(const void *elem) {
    const HeroArrayHeader *a = *(const HeroArrayHeader *const *)elem;
    hero_array_require(a);
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    const unsigned char *data = hero_array_data_const(a);
    for (int64_t i = 0; i < a->len; i++) {
        uint64_t one = a->elem->hash(data + (size_t)i * a->elem->size);
        h ^= one;
        h *= UINT64_C(0x100000001b3);
    }
    return h;
}

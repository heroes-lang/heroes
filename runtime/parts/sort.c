/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/sort.c — `sort(xs) -> [T]`, stable, ascending, for the three element
 * types that have an order.
 *
 * Two decisions live here and both were measured rather than argued
 * (panel 027): the comparison is INTERNAL to this translation unit rather than a
 * sixth `HeroDesc` field, and the algorithm is a hand-written bottom-up merge
 * sort rather than `qsort`. Each function's own comment carries its evidence.
 *
 * Depends on `parts/array.c` for the layout helpers and on `parts/desc.c` for
 * the three static descriptors it dispatches on by pointer identity.
 *
 * design.md §4.14, §4.20, panels 006 and 027.
 */

/* -- sort: the three element types that have an order -----------------------
 *
 * The comparison is INTERNAL — this typedef is deliberately not in
 * `heroes_runtime.h`. Nothing outside this file needs to name it, so changing it
 * is not an ABI event and no generated unit has to agree with it. That is the
 * whole shape of panel 027's veto: a sixth `HeroDesc` field would have been an
 * ABI event, and C11 6.7.9p21 zero-fills a short initialiser list, so every
 * descriptor that forgot it would carry a NULL and SEGV with no type name. */
typedef int64_t (*HeroCmpFn)(const void *, const void *);

static int64_t hero_cmp_int(const void *x, const void *y) {
    int64_t a = *(const int64_t *)x, b = *(const int64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0); /* never `a - b`: that overflows */
}

/* NaN ABORTS rather than being given an invented place, and that is the same
 * answer §4.14 already gives every other arithmetic edge — overflow aborts,
 * division by zero aborts. `<` is not a total order at NaN, so a comparison sort
 * driven by it produces an ARBITRARY permutation: a wrong answer with no error,
 * and one that can differ between two correct implementations, which the M-selfhost-fixpoint
 * fixpoint cannot have. IEEE 754 totalOrder (Rust's `total_cmp`, Go's NaN-first)
 * is the other way to be deterministic here; it is silent where this is loud,
 * and it would owe the spec a sentence about NaN, which the spec has never
 * needed. `-0.0` and `0.0` tie, and the sort is stable, so their order is the
 * input's. */
static int64_t hero_cmp_f64(const void *x, const void *y) {
    double a = *(const double *)x, b = *(const double *)y;
    if (a != a || b != b) hero_panic("sort of an f64 array containing nan");
    return a < b ? -1 : (a > b ? 1 : 0);
}

static int64_t hero_cmp_str(const void *x, const void *y) {
    return hero_str_cmp(*(const HeroStr *)x, *(const HeroStr *)y);
}

/* The other seven widths. Each compares AT ITS OWN WIDTH, and the unsigned ones
 * must: read as `int64_t`, a `u64` of 2^63 is negative and sorts before zero.
 * That is the whole reason these are seven functions rather than a widening
 * cast into `hero_cmp_int` (M-sized-integers, panel 042). */
static int64_t hero_cmp_i8(const void *x, const void *y) {
    int8_t a = *(const int8_t *)x, b = *(const int8_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_i16(const void *x, const void *y) {
    int16_t a = *(const int16_t *)x, b = *(const int16_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_i32(const void *x, const void *y) {
    int32_t a = *(const int32_t *)x, b = *(const int32_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_u8(const void *x, const void *y) {
    uint8_t a = *(const uint8_t *)x, b = *(const uint8_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_u16(const void *x, const void *y) {
    uint16_t a = *(const uint16_t *)x, b = *(const uint16_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_u32(const void *x, const void *y) {
    uint32_t a = *(const uint32_t *)x, b = *(const uint32_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}
static int64_t hero_cmp_u64(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

/* Dispatch is POINTER IDENTITY against the static descriptors, which is exact:
 * every `[i64]` in every program carries `&hero_desc_int`, and after
 * M-sized-integers every `[u8]` carries `&hero_desc_u8`. A descriptor arriving
 * here that is on none of these rows means the gate let through a `sort` on an
 * element type with no order, so the message says compiler bug rather than user
 * error. */
static HeroCmpFn hero_cmp_for(const HeroDesc *elem) {
    if (elem == &hero_desc_int) return hero_cmp_int;
    if (elem == &hero_desc_f64) return hero_cmp_f64;
    if (elem == &hero_desc_str) return hero_cmp_str;
    if (elem == &hero_desc_i8) return hero_cmp_i8;
    if (elem == &hero_desc_i16) return hero_cmp_i16;
    if (elem == &hero_desc_i32) return hero_cmp_i32;
    if (elem == &hero_desc_u8) return hero_cmp_u8;
    if (elem == &hero_desc_u16) return hero_cmp_u16;
    if (elem == &hero_desc_u32) return hero_cmp_u32;
    if (elem == &hero_desc_u64) return hero_cmp_u64;
    return NULL;
}

/* One stable merge of two adjacent runs. `< 0` on the right-hand element is what
 * keeps the left run first on a tie — that is what stability means, and it is
 * what makes the output a function of the input rather than of the merge order. */
static void hero_merge_two(unsigned char *dst, const unsigned char *lo, int64_t nl,
                           const unsigned char *hi, int64_t nh, size_t size,
                           HeroCmpFn cmp) {
    int64_t i = 0, j = 0, k = 0;
    while (i < nl && j < nh) {
        if (cmp(hi + (size_t)j * size, lo + (size_t)i * size) < 0) {
            memcpy(dst + (size_t)k * size, hi + (size_t)j * size, size);
            j += 1;
        } else {
            memcpy(dst + (size_t)k * size, lo + (size_t)i * size, size);
            i += 1;
        }
        k += 1;
    }
    if (i < nl) memcpy(dst + (size_t)k * size, lo + (size_t)i * size, (size_t)(nl - i) * size);
    if (j < nh) memcpy(dst + (size_t)k * size, hi + (size_t)j * size, (size_t)(nh - j) * size);
}

/* Bottom-up merge sort, and NOT `qsort`. Three reasons, heaviest first
 * (panel 027 R6, all three measured rather than argued):
 *
 * 1. `qsort` is NOT STABLE and its algorithm differs per platform — Darwin's
 *    returned `0/0 0/3 0/6 0/9 1/10 1/1 1/7 1/4` on equal keys. Two correct
 *    hosts would then emit two different generated files, and the self-hosting
 *    fixpoint compares bytes (panel 006's reason for the map's fixed seed).
 * 2. `qsort`'s comparator is `int (*)(const void *, const void *)`. A Heroes
 *    comparison returns `int64_t`, so passing one needs a cast between function
 *    pointer types, and CALLING through the wrong type is C11 6.3.2.3p8
 *    undefined behaviour — which `-fsanitize=function` does not catch.
 * 3. `qsort` with an inconsistent comparator may run off the array; a merge sort
 *    with the same comparator produces a wrong order and stays in bounds.
 *
 * The scratch buffer is one malloc/free pair balanced inside this call. It is
 * not a Heroes block, so `hero_live_blocks` never sees it. */
static void hero_sort_elems(unsigned char *base, int64_t n, size_t size, HeroCmpFn cmp) {
    if (n < 2) return;
    unsigned char *tmp = hero_alloc((size_t)n * size);
    if (tmp == NULL) hero_panic("out of memory");
    unsigned char *src = base;
    unsigned char *dst = tmp;
    for (int64_t width = 1; width < n; width *= 2) {
        if (width > INT64_MAX / 2) hero_panic("array too large to sort");
        for (int64_t i = 0; i < n; i += width * 2) {
            int64_t mid = i + width < n ? i + width : n;
            int64_t end = i + width * 2 < n ? i + width * 2 : n;
            hero_merge_two(dst + (size_t)i * size, src + (size_t)i * size, mid - i,
                           src + (size_t)mid * size, end - mid, size, cmp);
        }
        unsigned char *swap = src;
        src = dst;
        dst = swap;
    }
    if (src != base) memcpy(base, src, (size_t)n * size);
    hero_release(tmp);
}

/* `sort(xs) -> [T]` — a NEW array, exactly as `push` returns one: §4.10 has no
 * reading in which the argument is mutated, and `sort` is an expression. The
 * elements are copied through the descriptor first, so a `[str]` result owns its
 * own references; the merge then PERMUTES those copies with `memcpy`, which
 * touches no refcount at all. */
HeroArrayHeader *hero_array_sort(const HeroArrayHeader *a) {
    hero_array_require(a);
    HeroCmpFn cmp = hero_cmp_for(a->elem);
    if (cmp == NULL) {
        hero_panic("sort of an array whose element type has no order — "
                   "this is a compiler bug, please report it");
    }
    HeroArrayHeader *b = hero_array_new(a->elem, a->len);
    const unsigned char *src = hero_array_data_const(a);
    unsigned char *dst = hero_array_data(b);
    size_t size = a->elem->size;
    for (int64_t i = 0; i < a->len; i++) {
        a->elem->copy(dst + (size_t)i * size, src + (size_t)i * size);
    }
    b->len = a->len;
    hero_sort_elems(dst, b->len, size, cmp);
    return b;
}

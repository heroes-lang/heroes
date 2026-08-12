/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/desc.c — the four scalar descriptors the compiler never declares.
 *
 * Written here rather than generated because there is no declaration in any
 * source file to generate them from, and because there is exactly one right
 * answer per scalar. Every other descriptor in a program is generated
 * (`crates/heroes/src/emit/perfn.rs`).
 *
 * `hash` is present on all four and on every generated one, never null — a call
 * through a null `hash` is a SEGV with no type name and no source line
 * (panel 022). The struct has five members and gains no sixth; the reason is in
 * design.md §4.20 and in panel 027.
 */

/* -- the descriptor ABI: the four scalars the compiler never declares --------
 *
 * Written here rather than generated because the compiler has no declaration to
 * generate them from, and because there is exactly one right answer per scalar:
 * a plain assignment, no drop, C's own `==`, and a hash that is a function of
 * the bytes THAT MATTER — which for `f64` means going through the bit pattern,
 * and for `bool` means normalising, since a `_Bool` can only be 0 or 1 but the
 * padding around it in an array element cannot be assumed. */

static void hero_copy_int(void *dst, const void *src) { *(int64_t *)dst = *(const int64_t *)src; }
static void hero_drop_nothing(void *elem) { (void)elem; }
static bool hero_eq_int(const void *a, const void *b) {
    return *(const int64_t *)a == *(const int64_t *)b;
}
/* FNV-1a over the eight bytes: one function, and the same one `toolchain.rs`
 * uses for the build cache, so there is one hash in this project rather than
 * two that drift. */
static uint64_t hero_hash_bytes(const void *p, size_t n) {
    const unsigned char *b = p;
    uint64_t h = UINT64_C(0xcbf29ce484222325);
    for (size_t i = 0; i < n; i++) {
        h ^= b[i];
        h *= UINT64_C(0x100000001b3);
    }
    return h;
}
static uint64_t hero_hash_int(const void *elem) { return hero_hash_bytes(elem, sizeof(int64_t)); }

static void hero_copy_f64(void *dst, const void *src) { *(double *)dst = *(const double *)src; }
static bool hero_eq_f64(const void *a, const void *b) {
    return *(const double *)a == *(const double *)b;
}
/* Through the bits, and NOT through the double: -0.0 == 0.0 is true while their
 * bit patterns differ, so hashing the bytes of a double would give two equal
 * values two hashes. Normalising the zero is what keeps eq and hash agreeing —
 * the invariant a map depends on. */
static uint64_t hero_hash_f64(const void *elem) {
    double v = *(const double *)elem;
    if (v == 0.0) v = 0.0; /* collapses -0.0 */
    return hero_hash_bytes(&v, sizeof v);
}

static void hero_copy_bool(void *dst, const void *src) { *(bool *)dst = *(const bool *)src; }
static bool hero_eq_bool(const void *a, const void *b) {
    return *(const bool *)a == *(const bool *)b;
}
static uint64_t hero_hash_bool(const void *elem) {
    unsigned char one = *(const bool *)elem ? 1u : 0u;
    return hero_hash_bytes(&one, 1);
}

static void hero_copy_str(void *dst, const void *src) {
    HeroStr s = *(const HeroStr *)src;
    hero_str_incref(s);
    *(HeroStr *)dst = s;
}
static void hero_drop_str(void *elem) { hero_str_decref(*(HeroStr *)elem); }
static bool hero_eq_str(const void *a, const void *b) {
    return hero_str_eq(*(const HeroStr *)a, *(const HeroStr *)b);
}
static uint64_t hero_hash_str(const void *elem) {
    HeroStr s = *(const HeroStr *)elem;
    hero_str_require(s);
    return hero_hash_bytes(s.ptr, (size_t)s.len);
}

const HeroDesc hero_desc_int = {sizeof(int64_t), hero_copy_int, hero_drop_nothing,
                                hero_eq_int, hero_hash_int};
const HeroDesc hero_desc_f64 = {sizeof(double), hero_copy_f64, hero_drop_nothing,
                                hero_eq_f64, hero_hash_f64};
const HeroDesc hero_desc_bool = {sizeof(bool), hero_copy_bool, hero_drop_nothing,
                                 hero_eq_bool, hero_hash_bool};
const HeroDesc hero_desc_str = {sizeof(HeroStr), hero_copy_str, hero_drop_str,
                                hero_eq_str, hero_hash_str};


/* -- the seven other integer widths (M-sized-integers, panel 042) ---------
 *
 * One descriptor apiece, and they may NOT share `hero_desc_int`: `sort.c`
 * dispatches on a descriptor's *pointer identity* to pick a comparison, and a
 * shared descriptor would also carry `sizeof(int64_t)` — so a `[u8]` would copy,
 * compare and hash eight bytes where it owns one. The emitter's arm for this is
 * an exhaustive `match` on the width for exactly that reason.
 *
 * `hash` goes through the value's own bytes at its own width, never through a
 * widened `int64_t`: CLAUDE.md §7 requires eq and hash to agree, and two `u8`s
 * that are equal must hash equal whatever sits in the seven bytes beside them. */
static void hero_copy_i8(void *dst, const void *src) { *(int8_t *)dst = *(const int8_t *)src; }
static bool hero_eq_i8(const void *a, const void *b) { return *(const int8_t *)a == *(const int8_t *)b; }
static uint64_t hero_hash_i8(const void *elem) {
    int8_t v = *(const int8_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_i8 = {sizeof(int8_t), hero_copy_i8, hero_drop_nothing,
                                hero_eq_i8, hero_hash_i8};
static void hero_copy_i16(void *dst, const void *src) { *(int16_t *)dst = *(const int16_t *)src; }
static bool hero_eq_i16(const void *a, const void *b) { return *(const int16_t *)a == *(const int16_t *)b; }
static uint64_t hero_hash_i16(const void *elem) {
    int16_t v = *(const int16_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_i16 = {sizeof(int16_t), hero_copy_i16, hero_drop_nothing,
                                hero_eq_i16, hero_hash_i16};
static void hero_copy_i32(void *dst, const void *src) { *(int32_t *)dst = *(const int32_t *)src; }
static bool hero_eq_i32(const void *a, const void *b) { return *(const int32_t *)a == *(const int32_t *)b; }
static uint64_t hero_hash_i32(const void *elem) {
    int32_t v = *(const int32_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_i32 = {sizeof(int32_t), hero_copy_i32, hero_drop_nothing,
                                hero_eq_i32, hero_hash_i32};
static void hero_copy_u8(void *dst, const void *src) { *(uint8_t *)dst = *(const uint8_t *)src; }
static bool hero_eq_u8(const void *a, const void *b) { return *(const uint8_t *)a == *(const uint8_t *)b; }
static uint64_t hero_hash_u8(const void *elem) {
    uint8_t v = *(const uint8_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_u8 = {sizeof(uint8_t), hero_copy_u8, hero_drop_nothing,
                                hero_eq_u8, hero_hash_u8};
static void hero_copy_u16(void *dst, const void *src) { *(uint16_t *)dst = *(const uint16_t *)src; }
static bool hero_eq_u16(const void *a, const void *b) { return *(const uint16_t *)a == *(const uint16_t *)b; }
static uint64_t hero_hash_u16(const void *elem) {
    uint16_t v = *(const uint16_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_u16 = {sizeof(uint16_t), hero_copy_u16, hero_drop_nothing,
                                hero_eq_u16, hero_hash_u16};
static void hero_copy_u32(void *dst, const void *src) { *(uint32_t *)dst = *(const uint32_t *)src; }
static bool hero_eq_u32(const void *a, const void *b) { return *(const uint32_t *)a == *(const uint32_t *)b; }
static uint64_t hero_hash_u32(const void *elem) {
    uint32_t v = *(const uint32_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_u32 = {sizeof(uint32_t), hero_copy_u32, hero_drop_nothing,
                                hero_eq_u32, hero_hash_u32};
static void hero_copy_u64(void *dst, const void *src) { *(uint64_t *)dst = *(const uint64_t *)src; }
static bool hero_eq_u64(const void *a, const void *b) { return *(const uint64_t *)a == *(const uint64_t *)b; }
static uint64_t hero_hash_u64(const void *elem) {
    uint64_t v = *(const uint64_t *)elem;
    return hero_hash_bytes(&v, sizeof v);
}
const HeroDesc hero_desc_u64 = {sizeof(uint64_t), hero_copy_u64, hero_drop_nothing,
                                hero_eq_u64, hero_hash_u64};

/* -- the function pointer -------------------------------------------------
 *
 * ONE descriptor for every function type, the way `hero_desc_array` is one for
 * every `[T]`: a Heroes function value is a bare C function pointer (§1.11 — no
 * closures in v1, so nothing is captured and there is nothing to own), so copy is
 * an assignment, drop is nothing, and equality is pointer identity.
 *
 * `HeroFn` is a generic function-pointer type used only as a *storage* shape.
 * C guarantees round-tripping any function pointer through any other function
 * pointer type (C11 6.3.2.3p8); what it forbids is CALLING through the wrong one,
 * and nothing here calls. The emitter's typedefs are what a call goes through, so
 * clang still type-checks every one.
 */
typedef void (*HeroFn)(void);

static void hero_copy_func(void *dst, const void *src) {
    *(HeroFn *)dst = *(const HeroFn *)src;
}
static bool hero_eq_func(const void *a, const void *b) {
    return *(const HeroFn *)a == *(const HeroFn *)b;
}
static uint64_t hero_hash_func(const void *elem) {
    return hero_hash_bytes(elem, sizeof(HeroFn));
}

const HeroDesc hero_desc_func = {sizeof(HeroFn), hero_copy_func, hero_drop_nothing,
                                 hero_eq_func, hero_hash_func};

/* SPDX-License-Identifier: Apache-2.0, with the Heroes runtime exception:
 * see LICENSE-RUNTIME-EXCEPTION. A program compiled with Heroes carries
 * part of this runtime inside it and owes nothing for doing so. */
/* parts/text.c — where `str` and `[T]` meet: `chars` and `join`.
 *
 * `chars` is TOTAL over every byte string, and `join` is one allocation. The
 * pair buys a law with no exceptions — `join(chars(s), "") == s` for every `s` —
 * and the reason that law is worth more than an abort is in `chars`'s own
 * comment: a Heroes `str` is arbitrary bytes because `.cstr()` is the zero-copy
 * handoff to C, and the rule that a `str` is well-formed UTF-8 is enforced at
 * `hero_str_slice`, the one operation that can break it.
 *
 * design.md §4.20, design.md:809, §1.11, panel 027 R4.
 */

/* Unicode 15 Table 3-7, which is the only correct spelling of "well-formed": it
 * rejects overlong forms, the surrogate range ED A0..BF, and everything above
 * U+10FFFF. Returns the sequence length 1..4, or 0 for "no valid sequence starts
 * here". The overlong check is not pedantry — non-shortest forms are Unicode
 * Corrigendum #1's documented filter-bypass, where one process validates and
 * another decodes. */
static int hero_utf8_seq(const unsigned char *p, int64_t avail) {
    unsigned char c = p[0];
    if (c <= 0x7F) return 1;
    if (c >= 0xC2 && c <= 0xDF) {
        if (avail < 2 || p[1] < 0x80 || p[1] > 0xBF) return 0;
        return 2;
    }
    if (c >= 0xE0 && c <= 0xEF) {
        unsigned char lo = 0x80, hi = 0xBF;
        if (c == 0xE0) lo = 0xA0; /* no overlong three-byte forms */
        if (c == 0xED) hi = 0x9F; /* no surrogates U+D800..U+DFFF */
        if (avail < 3) return 0;
        if (p[1] < lo || p[1] > hi) return 0;
        if (p[2] < 0x80 || p[2] > 0xBF) return 0;
        return 3;
    }
    if (c >= 0xF0 && c <= 0xF4) {
        unsigned char lo = 0x80, hi = 0xBF;
        if (c == 0xF0) lo = 0x90; /* no overlong four-byte forms */
        if (c == 0xF4) hi = 0x8F; /* nothing above U+10FFFF */
        if (avail < 4) return 0;
        if (p[1] < lo || p[1] > hi) return 0;
        if (p[2] < 0x80 || p[2] > 0xBF) return 0;
        if (p[3] < 0x80 || p[3] > 0xBF) return 0;
        return 4;
    }
    return 0;
}

/* `chars(s) -> [str]`, and it is TOTAL: a byte that starts no well-formed
 * sequence becomes a one-byte `str` rather than an abort (panel 027 R4).
 *
 * A Heroes `str` is arbitrary bytes and has to be, because `.cstr()` is §4.20's
 * zero-copy handoff and C strings are arbitrary NUL-free bytes — a filename from
 * `readdir`, a latin-1 column from SQLite (measured: a real `text` column
 * returned `0xEF`, because SQLite does not validate UTF-8), a BLOB. So the rule
 * that a `str` is well-formed lives at `hero_str_slice`, which is the only
 * operation in the language that can BREAK one, and not here, which is merely
 * where the symptom would show.
 *
 * What total buys is a law with no exceptions:
 *     join(chars(s), "") == s   for every s
 * assertable over `heroes mutate`'s whole corpus (CLAUDE.md §9). Under an abort
 * the law is conditional, and a conditional law tests nothing. */
HeroArrayHeader *hero_str_chars(HeroStr s) {
    hero_str_require(s);
    const unsigned char *p = (const unsigned char *)s.ptr;
    int64_t count = 0;
    for (int64_t i = 0; i < s.len;) {
        int n = hero_utf8_seq(p + i, s.len - i);
        i += n > 0 ? n : 1;
        count += 1;
    }
    HeroArrayHeader *out = hero_array_new(&hero_desc_str, count);
    unsigned char *data = hero_array_data(out);
    int64_t at = 0;
    for (int64_t i = 0; i < s.len;) {
        int n = hero_utf8_seq(p + i, s.len - i);
        int64_t take = n > 0 ? n : 1;
        HeroStr one = hero_str_from_bytes(s.ptr + i, take);
        memcpy(data + (size_t)at * sizeof(HeroStr), &one, sizeof(HeroStr));
        at += 1;
        i += take;
    }
    out->len = at;
    return out;
}

/* `join(parts, sep) -> str` — design.md:1318's answer to O(n^2) concatenation:
 * sum the lengths, allocate ONCE, copy once. The element check is pointer
 * identity against the one static descriptor, so a `[int]` arriving here is
 * named as a compiler bug rather than misread as text. */
HeroStr hero_str_join(const HeroArrayHeader *parts, HeroStr sep) {
    hero_array_require(parts);
    hero_str_require(sep);
    if (parts->elem != &hero_desc_str) {
        hero_panic("join of an array that does not hold str — "
                   "this is a compiler bug, please report it");
    }
    if (parts->len == 0) return hero_str_empty();
    const HeroStr *items = (const HeroStr *)(const void *)hero_array_data_const(parts);
    int64_t total = 0;
    for (int64_t i = 0; i < parts->len; i++) {
        hero_str_require(items[i]);
        if (items[i].len > INT64_MAX - total) hero_panic("string length overflow");
        total += items[i].len;
    }
    for (int64_t i = 1; i < parts->len; i++) {
        if (sep.len > INT64_MAX - total) hero_panic("string length overflow");
        total += sep.len;
    }
    if (total == 0) return hero_str_empty();
    HeroStr r = hero_str_alloc(total);
    char *w = (char *)(void *)(uintptr_t)r.ptr;
    for (int64_t i = 0; i < parts->len; i++) {
        if (i > 0 && sep.len > 0) {
            memcpy(w, sep.ptr, (size_t)sep.len);
            w += sep.len;
        }
        memcpy(w, items[i].ptr, (size_t)items[i].len);
        w += items[i].len;
    }
    return r;
}

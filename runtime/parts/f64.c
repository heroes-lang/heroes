/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/f64.c — canonical `f64` rendering, and the numeric conversions.
 *
 * Rendering is locale-independent by construction (`uselocale` with a C locale,
 * not `setlocale`), because a program that prints `1,5` in one environment and
 * `1.5` in another cannot be a golden test — and the M8c fixpoint compares
 * generated C byte for byte.
 *
 * `to_int`'s range check is the one piece of arithmetic in this runtime that
 * was arrived at by testing thirteen values rather than by reasoning; the
 * comment on it says which two plausible spellings are wrong.
 *
 * design.md §4.9, spec lines 150 and 155, panels 021 and 027.
 */

/* -- f64 rendering (design.md §4.9, panel 021) -----------------------------
 *
 * ROUND-TRIP-EXACT, never "shortest": %.*g at increasing precision until
 * strtod() reads back the same bits. The distinction is not pedantic — 5e-324
 * renders 4.94065645841247e-324, which round-trips and is not the shortest
 * decimal, and Java shipped 1.9999999999999998E23 for eighteen years before
 * Schubfach to prove it matters.
 *
 * The subnormal branch is gnulib's, whose ftoastr is the widely-shipped
 * implementation of this technique (coreutils, Emacs): below DBL_MIN the loop
 * starts at precision 1, because %.15g of 1e-323 is
 * 9.88131291682493e-324 — fifteen significant digits for a value Python
 * renders 1e-323.
 *
 * LOCALE IS REMOVED STRUCTURALLY, not promised away. Under de_DE.UTF-8 the
 * ladder emits the malformed `0,1.0` — the decimal-point rider finds no `.` and
 * appends one — and the round-trip check CANNOT SEE IT, because strtod reads
 * the same locale and is wrong consistently. `setlocale` is process-wide and
 * §1.11 guarantees every real program links C libraries: gettext, glib and
 * ncurses all call it, and `extern function setlocale(...)` is one line of
 * Heroes. So the render runs inside a "C" locale window, which covers snprintf
 * AND strtod at once. PEP 331 is the shape of the hazard: CPython never called
 * setlocale, GTK+ did, CPython broke anyway. */

/* Created once, never destroyed: the process needs exactly one.
 *
 * A failure here is not survivable and must not be silent. The caller's guard
 * was `if (c != 0) previous = uselocale(c);` — so if `newlocale` ever returned
 * zero the render would run in the ambient locale and emit `3,5`, which is the
 * exact defect this file exists to prevent and which its own comment says the
 * round-trip check cannot catch. Aborting is the honest answer: the alternative
 * is a number that is not a number in any locale (2026-08-12, sweep 001 S9). */
static locale_t hero_c_locale(void) {
    static locale_t cached = (locale_t)0;
    if (cached == (locale_t)0) {
        cached = newlocale(LC_ALL_MASK, "C", (locale_t)0);
        if (cached == (locale_t)0) hero_panic("cannot create the C locale for rendering");
    }
    return cached;
}

static int hero_f64_render(char *buf, size_t cap, double v) {
    if (v != v) return snprintf(buf, cap, "nan");
    if (v == (double)INFINITY) return snprintf(buf, cap, "inf");
    if (v == -(double)INFINITY) return snprintf(buf, cap, "-inf");

    locale_t c = hero_c_locale();
    locale_t previous = (locale_t)0;
    if (c != (locale_t)0) previous = uselocale(c);

    double magnitude = v < 0 ? -v : v;
    int first = magnitude != 0.0 && magnitude < DBL_MIN ? 1 : DBL_DIG;
    int n = 0;
    for (int prec = first; prec <= 17; prec++) {
        n = snprintf(buf, cap, "%.*g", prec, v);
        if (n < 0 || (size_t)n >= cap) hero_panic("f64 render overflow");
        if (strtod(buf, NULL) == v) break;
    }
    /* The rider: a value with no fractional part still shows a point, so
     * print(1.0) is not indistinguishable from print(1). Under `1`, the
     * programs `print(price * to_f64(count))` and `print(5 * 2)` would emit
     * identical bytes and an accidental int->f64 drift would stay green in
     * every golden — which is what §4.3's no-implicit-conversions rule exists
     * to prevent. */
    if (strpbrk(buf, ".eE") == NULL) {
        int m = snprintf(buf + n, cap - (size_t)n, ".0");
        if (m < 0 || (size_t)(n + m) >= cap) hero_panic("f64 render overflow");
        n += m;
    }
    if (previous != (locale_t)0) uselocale(previous);
    return n;
}

void hero_print_f64(double v) {
    char buf[40];
    (void)hero_f64_render(buf, sizeof buf, v);
    fputs(buf, stdout);
}

HeroStr hero_f64_to_str(double v) {
    char buf[40];
    int n = hero_f64_render(buf, sizeof buf, v);
    HeroStr r = hero_str_alloc(n);
    memcpy((char *)(void *)(uintptr_t)r.ptr, buf, (size_t)n);
    return r;
}

HeroStr hero_int_to_str(int64_t v) {
    char buf[24];
    int n = snprintf(buf, sizeof buf, "%lld", (long long)v);
    HeroStr r = hero_str_alloc(n);
    memcpy((char *)(void *)(uintptr_t)r.ptr, buf, (size_t)n);
    return r;
}

HeroStr hero_bool_to_str(bool v) {
    /* static blocks: to_str of a bool allocates nothing */
    static const struct { HeroStrHeader h; char b[5]; } t = {{-1, HERO_STR_MAGIC}, "true"};
    static const struct { HeroStrHeader h; char b[6]; } f = {{-1, HERO_STR_MAGIC}, "false"};
    return v ? (HeroStr){t.b, 4} : (HeroStr){f.b, 5};
}

/* `to_str` of a `str` is the identity, and it must still hand back a reference so
 * that the caller's uniform "a call returns +1" rule holds. */
HeroStr hero_str_identity(HeroStr s) {
    hero_str_require(s);
    hero_str_incref(s);
    return s;
}

/* -- the numeric conversions (spec line 150) --------------------------------
 *
 * `(int64_t)v` where the truncated `v` is outside int64's range is UNDEFINED
 * BEHAVIOUR (C11 6.3.1.4p1), which CLAUDE.md §7 forbids reaching — so this check
 * is not a courtesy, it is the difference between an abort and whatever the
 * hardware felt like. On arm64 `fcvtzs` SATURATES, so the unchecked version
 * returns INT64_MAX for `inf` and 0 for NaN with no sanitiser saying a word.
 *
 * Three details, each of which a plausible rewrite gets wrong (panel 027 R7
 * tested thirteen values rather than reasoning about them):
 *
 *   - NEGATED, so NaN needs no clause of its own: NaN compares false against
 *     everything, so `!(...)` is true and it aborts.
 *   - HEX FLOAT bounds, because they are exact. `(double)INT64_MAX` rounds UP to
 *     2^63 and then reads as if it had not, so `v <= (double)INT64_MAX` ACCEPTS
 *     2^63 and is UB.
 *   - HALF-OPEN on the right, because 2^63-1 is not representable as a double:
 *     the largest acceptable value is the double just below 2^63. */
int64_t hero_f64_to_int(double v) {
    if (!(v >= -0x1p63 && v < 0x1p63)) {
        hero_panic("to_int of an f64 outside the range of int");
    }
    return (int64_t)v; /* truncates toward zero, as spec line 131 requires */
}

/* No range to check: every int64_t converts. It is lossy above 2^53 — round to
 * nearest, which is defined behaviour rather than UB — and that loss is silent.
 * Every language with these two types has it; Part 8 is where it belongs, and an
 * abort is not, because the value that loses precision is a legitimate `int`. */
double hero_int_to_f64(int64_t v) { return (double)v; }

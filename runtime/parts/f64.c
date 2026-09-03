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
 * `1.5` in another cannot be a golden test — and the M-selfhost-fixpoint fixpoint compares
 * generated C byte for byte.
 *
 * `to_i64`'s range check is the one piece of arithmetic in this runtime that
 * was arrived at by testing thirteen values rather than by reasoning; the
 * comment on it says which two plausible spellings are wrong.
 *
 * design.md §4.9, spec lines 150 and 155, panels 021 and 027.
 */

/* -- f64 rendering (design.md §4.9, panel 021) -----------------------------
 *
 * ROUND-TRIP-EXACT is the promise, and "shortest" is not promised: %.*g at
 * increasing precision until strtod() reads back the same bits. The promise
 * is the one a program may rely on; where the ladder also happens to give the
 * shortest decimal, that is an outcome and not a guarantee. Java shipped
 * 1.9999999999999998E23 for eighteen years before Schubfach; this printer
 * renders it 2e+23 (measured 2026-09-03, compiled directly).
 *
 * This comment said for a month that 5e-324 renders 4.94065645841247e-324.
 * It has rendered 5e-324 since the subnormal branch below landed
 * (DESIGN-LOG:100, 2026-08-05), and 1e-323 renders 1e-323 — both measured
 * 2026-09-03. A dead example in a comment reads as a live one (CLAUDE.md §11).
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
/* **Windows spells this better than POSIX does, and the port uses that.**
 *
 * POSIX 2008 gives `uselocale`, which *switches* the calling thread's locale and
 * must be switched back — so every early return between the two calls is a bug
 * waiting to be written. The Microsoft CRT instead takes the locale as an
 * argument to the conversion itself (`_snprintf_l`, `_strtod_l`), which removes
 * the window entirely rather than making it narrow. Both are wrapped below so
 * the rendering code reads the same on every platform.
 *
 * mingw has no `<xlocale.h>`, no `locale_t`, no `newlocale` — this is the whole
 * of what stopped the runtime compiling for Windows (18 errors, all in this
 * file, measured at panel 049). */
#if defined(_WIN32)
typedef _locale_t hero_locale;
#define HERO_NO_LOCALE ((_locale_t)0)
static hero_locale hero_make_c_locale(void) { return _create_locale(LC_ALL, "C"); }
#define hero_snprintf_c(buf, cap, loc, prec, v) _snprintf_s_l((buf), (cap), _TRUNCATE, "%.*g", (loc), (prec), (v))
#define hero_strtod_c(s, loc) _strtod_l((s), NULL, (loc))
#else
typedef locale_t hero_locale;
#define HERO_NO_LOCALE ((locale_t)0)
static hero_locale hero_make_c_locale(void) { return newlocale(LC_ALL_MASK, "C", (locale_t)0); }
#define hero_snprintf_c(buf, cap, loc, prec, v) snprintf((buf), (cap), "%.*g", (prec), (v))
#define hero_strtod_c(s, loc) strtod((s), NULL)
#endif

static hero_locale hero_c_locale(void) {
    static hero_locale cached = HERO_NO_LOCALE;
    if (cached == HERO_NO_LOCALE) {
        cached = hero_make_c_locale();
        if (cached == HERO_NO_LOCALE) hero_panic("cannot create the C locale for rendering");
    }
    return cached;
}

static int hero_f64_render(char *buf, size_t cap, double v) {
    if (v != v) return snprintf(buf, cap, "nan");
    if (v == (double)INFINITY) return snprintf(buf, cap, "inf");
    if (v == -(double)INFINITY) return snprintf(buf, cap, "-inf");

    hero_locale c = hero_c_locale();
#if !defined(_WIN32)
    /* POSIX: switch the thread's locale for the window below, and switch back.
     * On Windows nothing is switched — the locale travels as an argument. */
    locale_t previous = uselocale(c);
#endif

    double magnitude = v < 0 ? -v : v;
    int first = magnitude != 0.0 && magnitude < DBL_MIN ? 1 : DBL_DIG;
    int n = 0;
    for (int prec = first; prec <= 17; prec++) {
        n = hero_snprintf_c(buf, cap, c, prec, v);
        if (n < 0 || (size_t)n >= cap) hero_panic("f64 render overflow");
        if (hero_strtod_c(buf, c) == v) break;
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
#if !defined(_WIN32)
    if (previous != (locale_t)0) uselocale(previous);
#endif
    return n;
}

/* The `f32` mirror, and the two differences from the loop above are both facts
 * about binary32 rather than choices.
 *
 * `FLT_DIG` (6) starts the search and 9 ends it: 9 significant decimal digits
 * round-trip every binary32, as 17 do every binary64. And the round trip is
 * tested through a **cast**, `(float)strtod(...)`, not through `strtof`: the
 * decimal text goes to a double exactly — every 9-digit decimal is exact in
 * binary64 — and the cast back to float is the single rounding the comparison is
 * asking about. `strtof` would round once in the library and once more never,
 * which is the same answer here but depends on the library getting it right.
 *
 * The subnormal branch and the trailing `.0` rider are the `f64` loop's own,
 * for the reasons its comments give. */
static int hero_f32_render(char *buf, size_t cap, float v) {
    if (v != v) return snprintf(buf, cap, "nan");
    if (v == (float)INFINITY) return snprintf(buf, cap, "inf");
    if (v == -(float)INFINITY) return snprintf(buf, cap, "-inf");

    hero_locale c = hero_c_locale();
#if !defined(_WIN32)
    locale_t previous = uselocale(c);
#endif

    float magnitude = v < 0 ? -v : v;
    int first = magnitude != 0.0f && magnitude < FLT_MIN ? 1 : FLT_DIG;
    int n = 0;
    for (int prec = first; prec <= 9; prec++) {
        n = hero_snprintf_c(buf, cap, c, prec, (double)v);
        if (n < 0 || (size_t)n >= cap) hero_panic("f32 render overflow");
        if ((float)hero_strtod_c(buf, c) == v) break;
    }
    if (strpbrk(buf, ".eE") == NULL) {
        int m = snprintf(buf + n, cap - (size_t)n, ".0");
        if (m < 0 || (size_t)(n + m) >= cap) hero_panic("f32 render overflow");
        n += m;
    }
#if !defined(_WIN32)
    if (previous != (locale_t)0) uselocale(previous);
#endif
    return n;
}

void hero_print_f32(float v) {
    char buf[40];
    (void)hero_f32_render(buf, sizeof buf, v);
    fputs(buf, stdout);
}

HeroStr hero_f32_to_str(float v) {
    char buf[40];
    int n = hero_f32_render(buf, sizeof buf, v);
    HeroStr r = hero_str_alloc(n);
    memcpy((char *)(void *)(uintptr_t)r.ptr, buf, (size_t)n);
    return r;
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

/* The four float conversions the two widths need, all of them one cast.
 *
 * They exist as named entry points rather than as casts the emitter writes for
 * the reason every other primitive here does: `heroes_runtime.h` declares them,
 * so clang type-checks the call (CLAUDE.md §7). An emitted `(float)x` would be
 * unchecked text, and the one that matters — `hero_f64_to_f32` — is the only
 * conversion in this language that loses precision without failing. It is worth
 * having a name so a reader of the generated C can see it happen. */
float hero_int_to_f32(int64_t v) { return (float)v; }
float hero_f64_to_f32(double v) { return (float)v; }
double hero_f32_to_f64(float v) { return (double)v; }

HeroStr hero_int_to_str(int64_t v) {
    char buf[24];
    int n = snprintf(buf, sizeof buf, "%lld", (long long)v);
    HeroStr r = hero_str_alloc(n);
    memcpy((char *)(void *)(uintptr_t)r.ptr, buf, (size_t)n);
    return r;
}

/* `u64` is the one width that does not widen into an `int64_t` without losing a
 * value: 18446744073709551615 read as signed is -1, which is exactly what
 * `print(SIZE_MAX)` produced before this existed (panel 042, measured). The other
 * six narrow widths widen losslessly and go on using `hero_int_to_str`. */
HeroStr hero_uint_to_str(uint64_t v) {
    char buf[24];
    int n = snprintf(buf, sizeof buf, "%llu", (unsigned long long)v);
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
/* **Was an aborting conversion until 2026-08-13; now a question.** `to_i64` took
 * the `to_` scheme with the seven other widths and became fallible with them, so
 * the range check that used to end the program now decides a tag: the emitter
 * asks this, then builds an `ok` or a `fail`.
 *
 * The test is `v >= -0x1p63 && v < 0x1p63` and it is written that way because two
 * plausible spellings are WRONG: `v <= (double)INT64_MAX` accepts 2^63, since
 * that cast rounds UP, and `v > -9223372036854775809.0` rejects INT64_MIN. On
 * arm64 an unchecked cast does not trap — `fcvtzs` saturates — so this predicate
 * is the only thing between a caller and a silent INT64_MAX. */
bool hero_f64_fits_int(double v) { return v >= -0x1p63 && v < 0x1p63; }

int64_t hero_f64_to_int(double v) {
    return (int64_t)v; /* truncates toward zero, as the spec requires */
}

/* No range to check: every int64_t converts. It is lossy above 2^53 — round to
 * nearest, which is defined behaviour rather than UB — and that loss is silent.
 * Every language with these two types has it; Part 8 is where it belongs, and an
 * abort is not, because the value that loses precision is a legitimate `i64`. */
double hero_int_to_f64(int64_t v) { return (double)v; }

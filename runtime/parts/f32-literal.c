/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/f32-literal.c — what an `f32` literal holds: the compiler's door.
 *
 * The binary32 nearest a decimal's value, read ONCE, by `strtof`, as the `f32`
 * ladder of `parts/f64.c` reads its round trip, so the text a float prints and
 * the literal that text writes are read by one reader (defect 2276). The
 * compiler read every float literal with `atof` and wrote that double where an
 * `f32` stood, for C to narrow: two roundings, and at a decimal just off a
 * binary32 midpoint (25 significant bits, so a double) the narrowing's
 * tie-to-even takes the far neighbour. `0.00000000000000000000000007038531`
 * held 0x15ae43fe where 0x15ae43fd is nearest; over every decimal of at most
 * eight significant digits in binary32's range, nine are read so.
 *
 * Bound by `selfhost/emit/literal.hero` through `hero_compiler.h`, which
 * `runtime.c` includes before this file, so clang checks it against its
 * declaration. In the C locale for the reason the render is: a library that
 * sets another would move a literal. After `parts/f64.c`, whose locale window
 * and `strtof` spelling it shares. */

float hero_f32_of_decimal(const char *text) {
    if (text == NULL) hero_panic("an f32 literal with no text — a compiler bug");
    hero_locale c = hero_c_locale();
#if !defined(_WIN32)
    locale_t previous = uselocale(c);
#endif
    float v = hero_strtof_c(text, c);
#if !defined(_WIN32)
    if (previous != (locale_t)0) uselocale(previous);
#endif
    return v;
}

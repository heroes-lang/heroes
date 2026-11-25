/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_maths.h — the few functions of C's maths the platform
 * layer's members call, written here so that no member needs a library a
 * program's link does not carry (M-rebel-library, 2026-11-25).
 *
 * A HEADER OF THE MEMBERS ALONE: no Heroes group names it, and no header a
 * group names includes it. Each function is `static inline`, so a member that
 * includes it holds its own copy and no member of the archive defines it.
 *
 * WHY NOT <math.h>'s. A program's link carries the C library and the
 * libraries the program names, nothing else (`selfhost/cli/link.hero`), and
 * on a Linux target C's maths are a library of their own, libm, which a
 * program names with `link "m"`. A member cannot name it: libm on every link
 * line would build a program that binds `sqrt` without `link "m"`, which is
 * refused there today. So a member calling `nextafter` (the GTK arm's parts)
 * or `sin` (the audio member and its PulseAudio arm) linked only under
 * `--sanitize`, whose runtime brings libm, and was `internal error: linking
 * failed` at `-O0` and at `-O2` in the Linux arm64 image (measured 2026-11-25:
 * Debian clang 22.1.8, GNU ld 2.44, glibc 2.41, every member's undefined names
 * held against what the link's default libraries define). `floor` and `ceil`,
 * which arm64 compiles to one instruction each, are calls of libm on
 * x86-64's baseline. A test of the compiler links every member whole with
 * the runtime at every level (`selfhost/cli/layer.hero`), so a member calling
 * a name the link cannot find is red on the CI leg whose link lacks it.
 *
 * C'S ANSWER, BIT FOR BIT, where C's answer is exact: `hero_layer_floor`,
 * `hero_layer_ceil` and `hero_layer_next_toward` give it for every number,
 * the sign of a zero included, and a NaN for a NaN. `hero_layer_sin_of_turn`
 * is a tone's sample, the sine of a turn (a whole turn is 2 pi), reduced to a
 * quarter turn exactly and summed to its tenth term, within the bound its test
 * holds it to against C's `sin`.
 */
#ifndef HERO_LAYER_MATHS_H
#define HERO_LAYER_MATHS_H

#include <stdint.h>
#include <string.h>

/* 2^52: every double of this magnitude or more is a whole number, and every
 * one below it converts to `int64_t` and back exactly once its fraction is
 * cut. */
#define HERO_LAYER_WHOLE_FROM 4503599627370496.0

/* A zero carrying the sign of `like`, as C's `floor` and `ceil` give one. */
static inline double hero_layer_zero_like(double like) {
    uint64_t bits;
    double zero;

    memcpy(&bits, &like, sizeof bits);
    bits &= UINT64_C(0x8000000000000000);
    memcpy(&zero, &bits, sizeof zero);
    return zero;
}

/* C's `floor`: the largest whole number not above `x`. */
static inline double hero_layer_floor(double x) {
    double cut;
    double down;

    if (!(x > -HERO_LAYER_WHOLE_FROM && x < HERO_LAYER_WHOLE_FROM)) {
        return x; /* a NaN, an infinity, or a whole number already */
    }
    cut = (double)(int64_t)x; /* toward zero, exact below 2^52 */
    down = cut > x ? cut - 1.0 : cut;
    return down == 0.0 ? hero_layer_zero_like(x) : down;
}

/* C's `ceil`: the smallest whole number not below `x`. */
static inline double hero_layer_ceil(double x) {
    double cut;
    double up;

    if (!(x > -HERO_LAYER_WHOLE_FROM && x < HERO_LAYER_WHOLE_FROM)) {
        return x;
    }
    cut = (double)(int64_t)x;
    up = cut < x ? cut + 1.0 : cut;
    return up == 0.0 ? hero_layer_zero_like(x) : up;
}

/* C's `nextafter`: the double next to `x` in the direction of `toward`;
 * `toward` itself where the two are equal, and a NaN where either is one. */
static inline double hero_layer_next_toward(double x, double toward) {
    uint64_t bits;

    if (x != x || toward != toward) {
        return x + toward;
    }
    if (x == toward) {
        return toward;
    }
    if (x == 0.0) {
        bits = 1; /* the least subnormal, given `toward`'s sign below */
        memcpy(&x, &bits, sizeof x);
        return toward > 0.0 ? x : -x;
    }
    memcpy(&bits, &x, sizeof bits);
    /* Away from zero adds one to the magnitude's bits, toward it takes one,
     * and the largest finite double steps on to the infinity. */
    if ((x < toward) == (x > 0.0)) {
        bits += 1;
    } else {
        bits -= 1;
    }
    memcpy(&x, &bits, sizeof x);
    return x;
}

/* The sine of `turn` turns, sin(2 pi turn): 0 for a whole turn, a NaN for an
 * infinity or a NaN. The turn is cut to (-1/2, 1/2] and folded to a quarter
 * turn, each step exact, then the angle's Taylor sum is taken to x^19/19!,
 * whose next term is below 3e-16 at a quarter turn. */
static inline double hero_layer_sin_of_turn(double turn) {
    double r;
    double x;
    double x2;

    if (!(turn > -HERO_LAYER_WHOLE_FROM && turn < HERO_LAYER_WHOLE_FROM)) {
        return turn - turn;
    }
    r = turn - hero_layer_floor(turn); /* [0, 1), exact */
    if (r > 0.5) {
        r -= 1.0; /* (-1/2, 1/2], exact */
    }
    if (r > 0.25) {
        r = 0.5 - r; /* sin(pi - a) = sin(a), exact by Sterbenz */
    } else if (r < -0.25) {
        r = -0.5 - r;
    }
    x = r * 6.283185307179586;
    x2 = x * x;
    return x + x * x2 * (-1.0 / 6.0 + x2 * (1.0 / 120.0 + x2 * (-1.0 / 5040.0 + x2 * (1.0 / 362880.0 + x2 * (-1.0 / 39916800.0 + x2 * (1.0 / 6227020800.0 + x2 * (-1.0 / 1307674368000.0 + x2 * (1.0 / 355687428096000.0 + x2 * (-1.0 / 121645100408832000.0)))))))));
}

#endif

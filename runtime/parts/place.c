/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/place.c — where a stop happened (panel 230's R1, design.md §1.12, §4.17).
 *
 * Every abort a program's own operation raises names its place, beneath the
 * line it always wrote:
 *
 *     panic: integer overflow
 *       at shop.hero:12:15, in shop.total
 *
 * The file as the program's modules name it, the line, the column of the
 * expression whose evaluation stopped the program, and the function as a
 * caller names it, `<module>.<name>`, or `test "<title>"`. The emitter writes
 * the four as literals on the failing branch alone (`selfhost/emit/stop_place.hero`),
 * so the path that does not abort computes nothing and holds no record.
 *
 * NEW NAMES, never new parameters of an old one (the ffi-pragmatist's veto,
 * design.md §4.20): a removed or reshaped function is a link error, a function
 * whose parameters grew is a place read out of a register nobody set. So each
 * entry point below is an old one, `_at` added, its parameters the old ones and
 * the place last; the old ones stand while a seed calls them, and leave after
 * the release whose seed knows these (the sitting's landing, its step 10).
 *
 * WRITTEN FROM THE FIXED BUFFER `failure.c` holds, before anything allocates
 * (the critic's finding 6): an index, an overflow or a division abort allocates
 * nothing, and its report must not start to. The place's file and function are
 * the program's text, so each is a message, every character a line cannot show
 * written by its code (`hero_report_message`); its two numbers are written by
 * `hero_report_decimal`, which asks no locale and no heap. */

/* A text the emitter wrote as a C literal: never NULL from it, `?` if it were. */
static HeroStr hero_place_text(const char *text) {
    if (text == NULL) return (HeroStr){"?", 1};
    return (HeroStr){text, (int64_t)strlen(text)};
}

/* A number in decimal, from a buffer on the stack. */
static void hero_report_decimal(HeroReport *r, int64_t v) {
    char digits[24];
    size_t at = sizeof digits;
    uint64_t u = v < 0 ? (uint64_t)0 - (uint64_t)v : (uint64_t)v;
    do {
        digits[--at] = (char)('0' + (int)(u % 10));
        u /= 10;
    } while (u > 0);
    if (v < 0) digits[--at] = '-';
    hero_report_put(r, digits + at, sizeof digits - at);
}

/* The report's second line: `  at <file>:<line>:<column>, in <function>`. */
static void hero_report_place(HeroReport *r, const char *file, const char *function, int64_t line,
                              int64_t column) {
    HERO_REPORT_OWN(r, "  at ");
    hero_report_message(r, hero_place_text(file));
    HERO_REPORT_OWN(r, ":");
    hero_report_decimal(r, line);
    HERO_REPORT_OWN(r, ":");
    hero_report_decimal(r, column);
    HERO_REPORT_OWN(r, ", in ");
    hero_report_message(r, hero_place_text(function));
    HERO_REPORT_OWN(r, "\n");
}

/* `hero_panic`'s line, then the place. `msg` is the emitter's own words, a
 * division by zero or a shift's range; written as a message, which leaves
 * every one of them as `hero_panic` writes it and could not let one forge a
 * line if it held a line feed. */
_Noreturn void hero_panic_at(const char *msg, const char *file, const char *function, int64_t line,
                             int64_t column) {
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: ");
    hero_report_message(&r, hero_place_text(msg));
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_overflow_at(const char *file, const char *function, int64_t line, int64_t column) {
    hero_panic_at("integer overflow", file, function, line, column);
}

/* The failing branch of an array's element read or written in the program's C
 * (`emit/array_read.hero`, `emit/element_store.hero`). NULL or a released block
 * is the compiler's own mistake, and `hero_array_require` says so in the words it
 * always did, with no place; an index in range never reaches here, and if it
 * did the branch that called is the compiler's, said as one. */
_Noreturn void hero_panic_index_at(const HeroArrayHeader *a, int64_t index, const char *file,
                                   const char *function, int64_t line, int64_t column) {
    hero_array_require(a);
    if (index >= 0 && index < a->len) hero_unreachable();
    hero_panic_at("array index out of range", file, function, line, column);
}

/* The same for a `str`'s byte, `s[i]` (`emit/str_read.hero`). */
_Noreturn void hero_panic_byte_at(HeroStr s, int64_t index, const char *file, const char *function,
                                  int64_t line, int64_t column) {
    hero_str_require(s);
    if (index >= 0 && index < s.len) hero_unreachable();
    hero_panic_at("string index out of range", file, function, line, column);
}

/* `.must()` on an error: `hero_panic_must`'s line, then the place. */
_Noreturn void hero_panic_must_at(HeroFailure f, const char *file, const char *function, int64_t line,
                                  int64_t column) {
    hero_str_require(f.code);
    hero_str_require(f.msg);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: .must() on an error: ");
    hero_report_message(&r, f.code);
    HERO_REPORT_OWN(&r, ": ");
    hero_report_message(&r, f.msg);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

/* `assert`: the head line, the place, then the sides where there are two, so a
 * side's length never pushes the place out of a reader's first glance (panel
 * 230's R2, *called after the head line and the place are written*). */
_Noreturn void hero_panic_assert_at(HeroStr text, const char *file, const char *function, int64_t line,
                                    int64_t column) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_assert_sides_at(HeroStr text, HeroStr left, HeroStr right, const char *file,
                                          const char *function, int64_t line, int64_t column) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    HERO_REPORT_OWN(&r, "  left:  ");
    hero_report_message(&r, left);
    HERO_REPORT_OWN(&r, "\n  right: ");
    hero_report_message(&r, right);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_assert_str_sides_at(HeroStr text, HeroStr left, HeroStr right, const char *file,
                                              const char *function, int64_t line, int64_t column) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    HERO_REPORT_OWN(&r, "  left:  ");
    hero_report_value(&r, left);
    HERO_REPORT_OWN(&r, "\n  right: ");
    hero_report_value(&r, right);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

/* The control arm's `match` that no arm took (panel 226's R1): the case named as
 * `hero_panic_unmatched_case` names it, the value as `parts/failure.c`'s two
 * write it, then the place. */
_Noreturn void hero_panic_unmatched_case_at(int64_t index, HeroStr text, const char *file, const char *function,
                                            int64_t line, int64_t column) {
    hero_str_require(text);
    const char *p = text.ptr;
    const char *end_of = text.ptr + text.len;
    const char *start = p;
    int64_t k = 0;
    while (p < end_of && k < index) {
        if (*p == ',') {
            k++;
            start = p + 1;
        }
        p++;
    }
    const char *end = start;
    while (end < end_of && *end != ',') end++;
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: this match has no arm for .");
    hero_report_message(&r, (HeroStr){start, (int64_t)(end - start)});
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_unmatched_value_at(HeroStr text, const char *file, const char *function, int64_t line,
                                             int64_t column) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: this match has no arm for ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_unmatched_str_at(HeroStr text, const char *file, const char *function, int64_t line,
                                           int64_t column) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: this match has no arm for ");
    hero_report_value(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    hero_abort();
}

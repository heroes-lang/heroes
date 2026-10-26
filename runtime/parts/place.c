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

/* A place's words, `<file>:<line>:<column>, in <function>`, wherever a line
 * names one. */
static void hero_report_where(HeroReport *r, const char *file, const char *function, int64_t line,
                              int64_t column) {
    hero_report_message(r, hero_place_text(file));
    HERO_REPORT_OWN(r, ":");
    hero_report_decimal(r, line);
    HERO_REPORT_OWN(r, ":");
    hero_report_decimal(r, column);
    HERO_REPORT_OWN(r, ", in ");
    hero_report_message(r, hero_place_text(function));
}

/* The report's second line: `  at <file>:<line>:<column>, in <function>`. */
static void hero_report_place(HeroReport *r, const char *file, const char *function, int64_t line,
                              int64_t column) {
    HERO_REPORT_OWN(r, "  at ");
    hero_report_where(r, file, function, line, column);
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

/* Kind 2's funnel, declared in `parts/panic.c`: the record's four, or, for an
 * old entry point's NULL, `hero_panic` as it always was. */
_Noreturn static void hero_panic_where(const char *msg, const HeroPlace *where) {
    if (where == NULL) hero_panic(msg);
    hero_panic_at(msg, where->path, where->name, where->line, where->column);
}

/* Declared in `parts/panic.c`: the place's line after a report another part
 * wrote, from the fixed buffer, and nothing for NULL. */
static void hero_place_line(const HeroPlace *where) {
    if (where == NULL) return;
    HeroReport r = {.used = 0};
    hero_report_place(&r, where->path, where->name, where->line, where->column);
    hero_report_flush(&r);
}

/* Two places are one site where their four agree: a record is written per
 * site, so equal texts and numbers are one site, whichever copy of the record
 * was handed in. */
static bool hero_place_same(const HeroPlace *a, const HeroPlace *b) {
    if (a->line != b->line || a->column != b->column) return false;
    if ((a->path == NULL) != (b->path == NULL) || (a->name == NULL) != (b->name == NULL)) return false;
    if (a->path != NULL && a->path != b->path && strcmp(a->path, b->path) != 0) return false;
    return a->name == NULL || a->name == b->name || strcmp(a->name, b->name) == 0;
}

/* Declared in `parts/panic.c`, the first of the two passes over a table: it
 * keeps the HERO_PLACES_SHOWN places whose first taking came earliest, a later
 * place giving way to an earlier one. A place among the earliest never gives
 * way once its earliest entry is seen, since that would take as many places
 * taken before it as the list holds; so the pass ends holding exactly those,
 * each with its earliest order, whatever order the table is walked in. An
 * entry an old entry point gave no place is the second pass's to count. */
static void hero_places_see(HeroPlaces *t, const HeroPlace *where, uint64_t order) {
    if (where->path == NULL) return;
    size_t latest = 0;
    for (size_t k = 0; k < t->used; k++) {
        if (hero_place_same(&t->shown[k].where, where)) {
            if (order < t->shown[k].first) t->shown[k].first = order;
            return;
        }
        if (t->shown[k].first > t->shown[latest].first) latest = k;
    }
    HeroPlaceTally fresh = {*where, order, 0};
    if (t->used < HERO_PLACES_SHOWN) {
        t->shown[t->used++] = fresh;
    } else if (order < t->shown[latest].first) {
        t->shown[latest] = fresh;
    }
}

/* The second pass: how many entries each kept place holds, how many the table
 * holds, and how many of those name no place. */
static void hero_places_count(HeroPlaces *t, const HeroPlace *where) {
    t->seen++;
    if (where->path == NULL) {
        t->unplaced++;
        return;
    }
    for (size_t k = 0; k < t->used; k++) {
        if (hero_place_same(&t->shown[k].where, where)) {
            t->shown[k].count++;
            return;
        }
    }
}

/* The lines beneath an exit report's own, from the fixed buffer: each kept
 * place in the order its first was taken, how many were taken there; then the
 * entries taken at places past the list; then the ones no place was kept for,
 * an old entry point's and those `total`, the report's own count, holds past
 * the table (a lease's block C freed and the allocator handed out again, whose
 * entry the newer lease's took). */
static void hero_places_write(HeroPlaces *t, size_t total) {
    for (size_t k = 1; k < t->used; k++) {
        HeroPlaceTally hold = t->shown[k];
        size_t j = k;
        while (j > 0 && t->shown[j - 1].first > hold.first) {
            t->shown[j] = t->shown[j - 1];
            j--;
        }
        t->shown[j] = hold;
    }
    HeroReport r = {.used = 0};
    size_t listed = 0;
    for (size_t k = 0; k < t->used; k++) {
        const HeroPlace *w = &t->shown[k].where;
        HERO_REPORT_OWN(&r, "  ");
        hero_report_decimal(&r, (int64_t)t->shown[k].count);
        HERO_REPORT_OWN(&r, " taken at ");
        hero_report_where(&r, w->path, w->name, w->line, w->column);
        HERO_REPORT_OWN(&r, "\n");
        listed += t->shown[k].count;
    }
    size_t elsewhere = t->seen - t->unplaced - listed;
    size_t none = t->unplaced + (total > t->seen ? total - t->seen : 0);
    if (elsewhere > 0) {
        HERO_REPORT_OWN(&r, "  ");
        hero_report_decimal(&r, (int64_t)elsewhere);
        HERO_REPORT_OWN(&r, " more, taken elsewhere\n");
    }
    if (none > 0) {
        HERO_REPORT_OWN(&r, "  ");
        hero_report_decimal(&r, (int64_t)none);
        HERO_REPORT_OWN(&r, " more, taken where no place was kept\n");
    }
    hero_report_flush(&r);
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

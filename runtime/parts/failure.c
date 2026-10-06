/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/failure.c — the failure side of `T?`, and `.must()`'s abort.
 *
 * `HeroFailure` is the one record every `T?` in every program contains, which is
 * why it is declared in the public header rather than generated: §4.6 fixes its
 * shape, and there is no declaration in the source for the compiler to generate
 * it from.
 *
 * `hero_failure_missing_key` returns two STATIC blocks, so a map lookup that
 * misses allocates nothing — which matters because a miss is the common case in
 * a `.default(v)` chain.
 *
 * design.md §4.6, §4.20.
 */

void hero_failure_retain(const HeroFailure *v) {
    hero_str_incref(v->code);
    hero_str_incref(v->msg);
}

void hero_failure_release(HeroFailure *v) {
    hero_str_decref(v->code);
    hero_str_decref(v->msg);
}

bool hero_failure_eq(const HeroFailure *a, const HeroFailure *b) {
    /* The code first: §4.6 says a code is stable and a message is not, so a
     * comparison that short-circuits on the code is the one that keeps meaning
     * when somebody rewords the message. */
    return hero_str_eq(a->code, b->code) && hero_str_eq(a->msg, b->msg);
}

uint64_t hero_failure_hash(const void *elem) {
    const HeroFailure *v = elem;
    uint64_t h = hero_hash_str(&v->code);
    return (h ^ hero_hash_str(&v->msg)) * UINT64_C(0x100000001b3);
}

static void hero_copy_failure(void *dst, const void *src) {
    const HeroFailure *s = src;
    hero_failure_retain(s);
    *(HeroFailure *)dst = *s;
}
static void hero_drop_failure(void *elem) { hero_failure_release(elem); }
static bool hero_eq_failure(const void *a, const void *b) { return hero_failure_eq(a, b); }

const HeroDesc hero_desc_failure = {sizeof(HeroFailure), hero_copy_failure,
                                    hero_drop_failure, hero_eq_failure,
                                    hero_failure_hash};

/* The one failure `fit_<width>` produces. Static, like `missing_key`: a
 * conversion that does not fit allocates nothing, so the check is free to sit in
 * a loop (M-sized-integers, panel 042). */
HeroFailure hero_failure_does_not_fit(void) {
    HERO_STR_STATIC(code, "does_not_fit");
    HERO_STR_STATIC(msg, "the value is outside the target's range");
    /* `sizeof(b) - 1`, never a typed number — the rule the sibling below earned
     * the hard way, and the reason its comment is worth reading before touching
     * either message. */
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

HeroFailure hero_failure_missing_key(void) {
    HERO_STR_STATIC(code, "missing_key");
    HERO_STR_STATIC(msg, "no such key");
    /* `sizeof(b) - 1`, never a typed number. The lengths were `11` and `11`,
     * correct and unchecked: shortening either message would have shipped a
     * `str` claiming bytes past its own text, silently, while lengthening it is
     * a clang error — so the two directions failed differently and only one of
     * them loudly (2026-08-12, sweep 001 S12). */
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

/* **The same failure `read_file` and `validated` already give**, built here so
   the emitter can write `validated` over a fixed byte field inline (panel 162,
   M-readable-bytes). The code is `not_text` and it is the SAME STRING the
   library's `validated` and `hero_file_read` answer with, because panel 162
   measured that the rule was already chosen — invalid UTF-8 arriving from C is
   a recoverable failure carrying a stable snake_case code, never an abort and
   never a lossy decode — and a second spelling of one state is a second state
   as far as a program matching on `e.code` is concerned.

   `sizeof(b) - 1`, never a typed number, for the reason the sibling above
   earned the hard way: shortening a message with a typed length ships a `str`
   claiming bytes past its own text, silently, while lengthening it is a clang
   error, so the two directions fail differently and only one of them loudly.

   **And the array's size was typed too, which is the third direction** (defect
   275, 2026-10-04, found by panel 189's compiler-engineer): C lets a literal
   exactly as long as its array drop the NUL, with no diagnostic, so `char
   b[31]` held these 31 letters and no terminator. The `str` said 30 bytes,
   *these bytes are not valid UTF-*, and its `.cstr()` and the panic's `%s`
   read on past the array. Every static text of the runtime is now
   `HERO_STR_STATIC`, sized by `sizeof` of its own literal, and `HERO_STR_LIT`,
   so no number about a text is typed anywhere. */
HeroFailure hero_failure_not_text(void) {
    HERO_STR_STATIC(code, "not_text");
    HERO_STR_STATIC(msg, "these bytes are not valid UTF-8");
    return (HeroFailure){HERO_STR_LIT(code), HERO_STR_LIT(msg)};
}

/* -- the one printer of a failure's line (panel 192's R7, defect 355) ---------
 *
 * A failure's line holds the PROGRAM's texts, a `.must()`'s code and message
 * and an `assert`'s expression and sides, and it reaches a terminal. Two things
 * were wrong with writing them by `%s`, each measured by panel 192:
 *
 * - **by length**: `%s`, and `%.*s` too, stop at a NUL, and a `str` may hold
 *   one (panel 192's R1), so a message holding `a`, NUL, `b` printed `a`;
 * - **by code**: a control character went to the terminal as itself, so a
 *   message holding ESC `[2J` cleared the screen of whoever read it.
 *
 * So each piece is written by its length, and every character a line cannot
 * show is written by its code between angle brackets, `<U+001B>`, the form a
 * compiler's diagnostic writes it in (`selfhost/shown_char.hero`, `visible`):
 * a control character but the line feed and the tab, one of the twelve
 * bidirectional controls, which reorder the line a terminal draws, and U+2028
 * and U+2029. A `str` is well-formed UTF-8, so a sequence is read whole. The
 * line is built first and written in one `fwrite`, so it stays one write, as
 * the `fprintf` it replaces was. */
static int64_t hero_failure_unshowable(const unsigned char *p, int64_t left, int64_t *width) {
    unsigned char b = p[0];
    *width = 1;
    if (b < 0x80) return (b < 0x20 && b != '\n' && b != '\t') || b == 0x7f ? (int64_t)b : -1;
    if (b == 0xc2 && left >= 2) {
        *width = 2;
        return p[1] >= 0x80 && p[1] < 0xa0 ? (int64_t)p[1] : -1;
    }
    if (b == 0xd8 && left >= 2) {
        *width = 2;
        return p[1] == 0x9c ? 0x061c : -1;
    }
    if (b == 0xe2 && left >= 3) {
        *width = 3;
        int64_t code = ((int64_t)(b & 0x0f) << 12) | ((int64_t)(p[1] & 0x3f) << 6) | (int64_t)(p[2] & 0x3f);
        bool bidi = code == 0x200e || code == 0x200f || (code >= 0x202a && code <= 0x202e) ||
                    (code >= 0x2066 && code <= 0x2069);
        return bidi || code == 0x2028 || code == 0x2029 ? code : -1;
    }
    if (b >= 0xc0) *width = b >= 0xf0 ? 4 : b >= 0xe0 ? 3 : 2;
    if (*width > left) *width = left;
    return -1;
}

/* Writes the pieces into `line`, or only counts them where `line` is NULL. */
static size_t hero_failure_shown(const HeroStr *pieces, int count, char *line) {
    size_t at = 0;
    for (int i = 0; i < count; i++) {
        const unsigned char *p = (const unsigned char *)pieces[i].ptr;
        int64_t k = 0;
        while (k < pieces[i].len) {
            int64_t width = 1;
            int64_t code = hero_failure_unshowable(p + k, pieces[i].len - k, &width);
            if (code >= 0) {
                char name[16];
                int n = snprintf(name, sizeof name, "<U+%04llX>", (unsigned long long)code);
                if (line != NULL) memcpy(line + at, name, (size_t)n);
                at += (size_t)n;
            } else {
                if (line != NULL) memcpy(line + at, p + k, (size_t)width);
                at += (size_t)width;
            }
            k += width;
        }
    }
    return at;
}

static void hero_failure_write(const HeroStr *pieces, int count) {
    size_t total = hero_failure_shown(pieces, count, NULL);
    char *line = hero_alloc(total + 1);
    size_t wrote = hero_failure_shown(pieces, count, line);
    fwrite(line, 1, wrote, stderr);
    hero_release(line);
}

/* A text of the runtime's own, a piece beside the program's. */
#define HERO_FAILURE_TEXT(text) ((HeroStr){(text), (int64_t)(sizeof(text) - 1)})

_Noreturn void hero_panic_must(HeroFailure f) {
    /* Built by hand rather than through `hero_panic`: `code` and `msg` come
     * from the program, so they go through the printer above. */
    fflush(stdout);
    HeroStr pieces[5] = {HERO_FAILURE_TEXT("panic: .must() on an error: "), f.code,
                         HERO_FAILURE_TEXT(": "), f.msg, HERO_FAILURE_TEXT("\n")};
    hero_failure_write(pieces, 5);
    hero_abort();
}

/* -- `assert` (§4.18, spec § 12 Tests and holes) ---------------------------------------
 *
 * "An `assert` failure shows the source expression and both sides." Both halves
 * matter and the second is the one a bare `assert failed` loses: `dist2(3, 4)
 * == 25` tells the reader what was claimed, and `26 != 25` tells them what
 * happened. Two entry points rather than optional arguments, because C has no
 * optional arguments and a NULL `HeroStr` is the one non-value every entry point
 * here rejects.
 *
 * It is a PANIC, not a return: a test that fails stops. `heroes test` runs each
 * test in its own process for exactly that reason, so one failure does not hide
 * the tests after it. */
_Noreturn void hero_panic_assert(HeroStr text) {
    hero_str_require(text);
    fflush(stdout);
    HeroStr pieces[3] = {HERO_FAILURE_TEXT("assert failed: "), text, HERO_FAILURE_TEXT("\n")};
    hero_failure_write(pieces, 3);
    hero_abort();
}

_Noreturn void hero_panic_assert_sides(HeroStr text, HeroStr left, HeroStr right) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    HeroStr pieces[7] = {HERO_FAILURE_TEXT("assert failed: "), text, HERO_FAILURE_TEXT("\n  left:  "),
                         left, HERO_FAILURE_TEXT("\n  right: "), right, HERO_FAILURE_TEXT("\n")};
    hero_failure_write(pieces, 7);
    hero_abort();
}

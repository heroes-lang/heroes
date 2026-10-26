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

/* -- the one printer of a failure's report (panel 192's R7, defect 355; panel 230's R3) --
 *
 * A report holds three kinds of text, and each is written its own way:
 *
 * - **the runtime's own words**, `panic: `, `\n  left:  `, written as they are,
 *   line feeds included, since they are the report's lines;
 * - **a program's message**, a `.must()`'s code and message or an `assert`'s
 *   source text: every character a line cannot show written by its code between
 *   angle brackets, `<U+001B>`, the form the compiler's diagnostics write program
 *   text in (`selfhost/shown_char.hero`, `visible`). **The line feed and the tab
 *   included since panel 230's R3**: panel 192's R7 left both as themselves, and
 *   a line feed is the one character that lets a program's text impersonate a
 *   line of the report, a second `panic:` or a forged `right:` (defect 2225);
 * - **a program's `str` value**, an `assert`'s side or an unmatched value of the
 *   control arm: quoted, as the specification writes a literal, `\n` `\t` `\r`
 *   `\\` `\"`, and `\u{1b}` for any other character a line cannot show, `\u{0}`
 *   for a NUL (the lldb formatter's spelling, `editors/lldb/heroes_lldb.py`), at
 *   most TEXT_BYTES of it, then ` ... N more bytes`, as that formatter cuts.
 *
 * So a value can be pasted back into a program as a literal, a message reads as
 * the compiler writes program text, and neither can pass for a line of the
 * report. Each piece is read by its length, never to a NUL (panel 192's R1).
 *
 * **Written from a fixed buffer on the stack, never allocating** (panel 230's
 * critic, finding 6): an index, an overflow or a division abort allocates
 * nothing, and its report must not start to on its failing path, where an
 * exhausted heap would otherwise put `out of memory` in place of the cause. A
 * report longer than the buffer leaves in more than one write; a short one, in
 * one, as the `fprintf` this replaced did. */

/* The code of a character a line cannot show, or -1; `*width` its length in
 * bytes. A control character (the line feed and the tab included), DEL, a C1
 * control, one of the twelve bidirectional controls, U+2028 and U+2029. A `str` is
 * well-formed UTF-8, so a sequence is read whole. */
static int64_t hero_report_unshowable(const unsigned char *p, int64_t left, int64_t *width) {
    unsigned char b = p[0];
    *width = 1;
    if (b < 0x80) return b < 0x20 || b == 0x7f ? (int64_t)b : -1;
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

#define HERO_REPORT_BUFFER 1024
#define HERO_REPORT_TEXT_BYTES 1024

typedef struct {
    char bytes[HERO_REPORT_BUFFER];
    size_t used;
} HeroReport;

static void hero_report_flush(HeroReport *r) {
    if (r->used > 0) fwrite(r->bytes, 1, r->used, stderr);
    r->used = 0;
}

static void hero_report_put(HeroReport *r, const char *p, size_t n) {
    while (n > 0) {
        if (r->used == HERO_REPORT_BUFFER) hero_report_flush(r);
        size_t room = HERO_REPORT_BUFFER - r->used;
        size_t take = n < room ? n : room;
        memcpy(r->bytes + r->used, p, take);
        r->used += take;
        p += take;
        n -= take;
    }
}

/* The runtime's own words, a literal: `sizeof`, never a typed number. */
#define HERO_REPORT_OWN(r, text) hero_report_put((r), (text), sizeof(text) - 1)

/* A program's message: every character a line cannot show by its code. */
static void hero_report_message(HeroReport *r, HeroStr s) {
    const unsigned char *p = (const unsigned char *)s.ptr;
    int64_t k = 0;
    while (k < s.len) {
        int64_t width = 1;
        int64_t code = hero_report_unshowable(p + k, s.len - k, &width);
        if (code >= 0) {
            char name[16];
            int n = snprintf(name, sizeof name, "<U+%04llX>", (unsigned long long)code);
            hero_report_put(r, name, (size_t)n);
        } else {
            hero_report_put(r, (const char *)p + k, (size_t)width);
        }
        k += width;
    }
}

/* A program's `str` value, quoted in the specification's escapes and cut where
 * the lldb formatter cuts, a character never split by the cut. */
static void hero_report_value(HeroReport *r, HeroStr s) {
    const unsigned char *p = (const unsigned char *)s.ptr;
    HERO_REPORT_OWN(r, "\"");
    int64_t k = 0;
    while (k < s.len) {
        int64_t width = 1;
        int64_t code = hero_report_unshowable(p + k, s.len - k, &width);
        if (k + width > HERO_REPORT_TEXT_BYTES) break;
        if (code == '\n') {
            HERO_REPORT_OWN(r, "\\n");
        } else if (code == '\t') {
            HERO_REPORT_OWN(r, "\\t");
        } else if (code == '\r') {
            HERO_REPORT_OWN(r, "\\r");
        } else if (code >= 0) {
            char name[16];
            int n = snprintf(name, sizeof name, "\\u{%llx}", (unsigned long long)code);
            hero_report_put(r, name, (size_t)n);
        } else if (p[k] == '"') {
            HERO_REPORT_OWN(r, "\\\"");
        } else if (p[k] == '\\') {
            HERO_REPORT_OWN(r, "\\\\");
        } else {
            hero_report_put(r, (const char *)p + k, (size_t)width);
        }
        k += width;
    }
    HERO_REPORT_OWN(r, "\"");
    if (k < s.len) {
        char more[48];
        int n = snprintf(more, sizeof more, " ... %lld more bytes", (long long)(s.len - k));
        hero_report_put(r, more, (size_t)n);
    }
}

_Noreturn void hero_panic_must(HeroFailure f) {
    /* `code` and `msg` come from the program, so they are messages, read as every
     * reader reads a str (defect 462). */
    hero_str_require(f.code);
    hero_str_require(f.msg);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: .must() on an error: ");
    hero_report_message(&r, f.code);
    HERO_REPORT_OWN(&r, ": ");
    hero_report_message(&r, f.msg);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

/* -- `assert` (§4.18, spec § 12 Tests and holes) ---------------------------------------
 *
 * "An `assert` failure shows the source expression and both sides." Both halves
 * matter and the second is the one a bare `assert failed` loses: `dist2(3, 4)
 * == 25` tells the reader what was claimed, and `26 != 25` tells them what
 * happened. Separate entry points rather than optional arguments, because C has
 * no optional arguments and a NULL `HeroStr` is the one non-value every entry
 * point here rejects.
 *
 * It is a PANIC, not a return: a test that fails stops. `heroes test` runs each
 * test in its own process for exactly that reason, so one failure does not hide
 * the tests after it. */
_Noreturn void hero_panic_assert(HeroStr text) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

/* The sides as `print` renders them: a number or a `bool`, and, from a compiler
 * older than panel 230's R3, a `str` side too, which is then written as a message
 * and so still cannot forge a line. */
_Noreturn void hero_panic_assert_sides(HeroStr text, HeroStr left, HeroStr right) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n  left:  ");
    hero_report_message(&r, left);
    HERO_REPORT_OWN(&r, "\n  right: ");
    hero_report_message(&r, right);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_assert_str_sides(HeroStr text, HeroStr left, HeroStr right) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n  left:  ");
    hero_report_value(&r, left);
    HERO_REPORT_OWN(&r, "\n  right: ");
    hero_report_value(&r, right);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

/* -- the control arm's unmatched value (panel 226's R1; panel 230's R3, defect 2226)
 *
 * A `match` on an integer or a string that no arm took, in a program built with
 * `--permissive`: the value rendered as `print` renders it. Moved here from
 * `parts/panic.c` on 2026-10-26, where it wrote the value by `%s`: a value holding
 * a line feed printed a line of its own, an ESC reached the terminal as itself,
 * and a NUL stopped the text. An integer's digits are a message; a `str` is a
 * value, quoted. */
_Noreturn void hero_panic_unmatched_value(HeroStr text) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: this match has no arm for ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

_Noreturn void hero_panic_unmatched_str(HeroStr text) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "panic: this match has no arm for ");
    hero_report_value(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_flush(&r);
    hero_abort();
}

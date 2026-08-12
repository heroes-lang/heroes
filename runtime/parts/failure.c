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

HeroFailure hero_failure_missing_key(void) {
    static const struct { HeroStrHeader h; char b[12]; } code = {
        {-1, HERO_STR_MAGIC}, "missing_key"};
    static const struct { HeroStrHeader h; char b[12]; } msg = {
        {-1, HERO_STR_MAGIC}, "no such key"};
    /* `sizeof(b) - 1`, never a typed number. The lengths were `11` and `11`,
     * correct and unchecked: shortening either message would have shipped a
     * `str` claiming bytes past its own text, silently, while lengthening it is
     * a clang error — so the two directions failed differently and only one of
     * them loudly (2026-08-12, sweep 001 S12). */
    return (HeroFailure){{code.b, (int64_t)sizeof(code.b) - 1},
                         {msg.b, (int64_t)sizeof(msg.b) - 1}};
}

_Noreturn void hero_panic_must(HeroFailure f) {
    /* Built by hand rather than through `hero_panic`, so the two strings print
     * without needing a NUL-terminated join: a `HeroStr` always has its NUL, but
     * `code` and `msg` come from the program and one `fprintf` is one write. */
    fflush(stdout);
    fprintf(stderr, "panic: .must() on an error: %s: %s\n",
            hero_str_cstr(f.code), hero_str_cstr(f.msg));
    abort();
}

/* -- `assert` (§4.18, spec line 163) ---------------------------------------
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
    fprintf(stderr, "assert failed: %s\n", hero_str_cstr(text));
    abort();
}

_Noreturn void hero_panic_assert_sides(HeroStr text, HeroStr left, HeroStr right) {
    hero_str_require(text);
    hero_str_require(left);
    hero_str_require(right);
    fflush(stdout);
    fprintf(stderr, "assert failed: %s\n  left:  %s\n  right: %s\n",
            hero_str_cstr(text), hero_str_cstr(left), hero_str_cstr(right));
    abort();
}

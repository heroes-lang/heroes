/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/show.c — the two sides of a failed `assert`, each written as the
 * specification writes the value (panel 230's R2, design.md §4.18, spec § 12).
 *
 * A record as its construction, `Point(x: 3, y: 4)`; a variant `.case(f: v)` or
 * `.case`; `[T]` and a fixed array `[e0, e1]`; `{K: V}` with its entries in the
 * order of their whole text; `T?` as `ok(v)`, `ok()` or `fail(code: "...",
 * msg: "...")`; a `str` quoted in the specification's escapes; a number and a
 * `bool` as `print` writes them; a `ptr`, a `cstr`, a function value and a
 * handle as their address, never read through (the ffi-pragmatist's veto: what C
 * put there is not this runtime's to read).
 *
 * ONE NOTATION WITH THE LLDB FORMATTER, `editors/lldb/heroes_lldb.py`, and its
 * four bounds: SHOWN elements of a container, then `... N more`; TEXT_BYTES of a
 * `str`, then `... N more bytes` (`failure.c`'s quoting, the report's own, so the
 * notation has one copy); BUDGET values in all and DEPTH levels, then `...`. The
 * top value takes no budget and every value inside it takes one, as the
 * formatter's summary does. The `lldb` suite holds the two to one corpus.
 *
 * The renderer of a type the program declares, or of a container, is the
 * emitter's (`emit/sides.hero`): one per type a unit's `assert`s compare, closed
 * over what those hold. The ones here are the runtime's types and the walks
 * every container shares. All have one shape, `HeroShowFn`: the side's text so
 * far, and the address of the value.
 *
 * Called on the failing path alone, AFTER the head line and the place are out of
 * the fixed buffer (`hero_panic_assert_shown_at`), so a renderer that meets a
 * heap that is gone loses the sides and never the line that says which `assert`
 * failed (the compiler-engineer's question 5). */

#define HERO_SHOW_SHOWN 16
#define HERO_SHOW_BUDGET 256
#define HERO_SHOW_DEPTH 6

struct HeroShow {
    char *bytes; /* the text so far, scratch of `hero_alloc` */
    size_t used;
    size_t room;
    int64_t values; /* values written inside the top one, against BUDGET */
    int64_t depth;  /* the level the next value is written at, against DEPTH */
};

static void hero_show_put(HeroShow *o, const char *p, size_t n) {
    if (n == 0) return;
    if (o->used + n > o->room) {
        size_t room = o->room > 0 ? o->room : 128;
        while (o->used + n > room) room *= 2;
        char *fresh = hero_alloc(room);
        if (o->used > 0) memcpy(fresh, o->bytes, o->used);
        if (o->bytes != NULL) hero_release(o->bytes);
        o->bytes = fresh;
        o->room = room;
    }
    memcpy(o->bytes + o->used, p, n);
    o->used += n;
}

/* The quoting's sink (`parts/failure.c`), here a side's text. */
static void hero_show_sink(void *sink, const char *p, size_t n) { hero_show_put((HeroShow *)sink, p, n); }

void hero_show_text(HeroShow *o, const char *text) { hero_show_put(o, text, strlen(text)); }

/* A text `print`'s own entry point made, written and given back. */
static void hero_show_made(HeroShow *o, HeroStr s) {
    hero_show_put(o, s.ptr, (size_t)s.len);
    hero_str_decref(s);
}

/* The level a container's values are written at, opened before them and closed
 * after. */
void hero_show_open(HeroShow *o) { o->depth++; }
void hero_show_close(HeroShow *o) { o->depth--; }

/* One value inside another: the formatter's `show`. A unit of the budget is
 * taken first, then a value past BUDGET or deeper than DEPTH is written `...`. */
void hero_show_child(HeroShow *o, HeroShowFn show, const void *p) {
    o->values++;
    if (o->values > HERO_SHOW_BUDGET || o->depth > HERO_SHOW_DEPTH) {
        hero_show_text(o, "...");
        return;
    }
    show(o, p);
}

/* `, ... N more`, or `... N more` where nothing was shown before it. */
static void hero_show_more(HeroShow *o, bool after, int64_t more) {
    char text[48];
    int n = snprintf(text, sizeof text, "%s... %lld more", after ? ", " : "", (long long)more);
    hero_show_put(o, text, (size_t)n);
}

void hero_show_i8(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const int8_t *)p)); }
void hero_show_i16(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const int16_t *)p)); }
void hero_show_i32(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const int32_t *)p)); }
void hero_show_i64(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const int64_t *)p)); }
void hero_show_u8(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const uint8_t *)p)); }
void hero_show_u16(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const uint16_t *)p)); }
void hero_show_u32(HeroShow *o, const void *p) { hero_show_made(o, hero_int_to_str(*(const uint32_t *)p)); }
void hero_show_u64(HeroShow *o, const void *p) { hero_show_made(o, hero_uint_to_str(*(const uint64_t *)p)); }
void hero_show_f32(HeroShow *o, const void *p) { hero_show_made(o, hero_f32_to_str(*(const float *)p)); }
void hero_show_f64(HeroShow *o, const void *p) { hero_show_made(o, hero_f64_to_str(*(const double *)p)); }
void hero_show_bool(HeroShow *o, const void *p) { hero_show_made(o, hero_bool_to_str(*(const bool *)p)); }

/* A `str` quoted, or the runtime's word for the slot no value is in. */
void hero_show_str(HeroShow *o, const void *p) {
    HeroStr s = *(const HeroStr *)p;
    if (s.ptr == NULL) {
        hero_show_text(o, "<unassigned>");
        return;
    }
    hero_quote_value(s, hero_show_sink, o);
}

/* An address, never read through: `nullptr`, the literal a program writes for
 * none, or `<0x...>`, the formatter's notation for an address it will not read.
 * Read by bytes, since a function value's slot is no `void *`. */
void hero_show_pointer(HeroShow *o, const void *p) {
    const void *at = NULL;
    memcpy(&at, p, sizeof at);
    if (at == NULL) {
        hero_show_text(o, "nullptr");
        return;
    }
    char text[40];
    int n = snprintf(text, sizeof text, "<0x%llx>", (unsigned long long)(uintptr_t)at);
    hero_show_put(o, text, (size_t)n);
}

void hero_show_unit(HeroShow *o, const void *p) {
    (void)p;
    hero_show_text(o, "()");
}

/* A failure as the formatter writes one, `(code: "...", msg: "...")`; a `T?`'s
 * failing side writes `fail` before it. */
void hero_show_failure(HeroShow *o, const void *p) {
    const HeroFailure *f = p;
    hero_show_text(o, "(code: ");
    hero_show_open(o);
    hero_show_child(o, hero_show_str, &f->code);
    hero_show_text(o, ", msg: ");
    hero_show_child(o, hero_show_str, &f->msg);
    hero_show_close(o);
    hero_show_text(o, ")");
}

/* SHOWN elements of `count` at `first`, each `size` bytes, then how many more. */
static void hero_show_elements(HeroShow *o, const unsigned char *first, int64_t count, size_t size,
                               HeroShowFn elem) {
    hero_show_text(o, "[");
    hero_show_open(o);
    int64_t shown = count < HERO_SHOW_SHOWN ? count : HERO_SHOW_SHOWN;
    for (int64_t i = 0; i < shown; i++) {
        if (i > 0) hero_show_text(o, ", ");
        hero_show_child(o, elem, first + (size_t)i * size);
    }
    if (count > shown) hero_show_more(o, shown > 0, count - shown);
    hero_show_close(o);
    hero_show_text(o, "]");
}

/* `[T]`, at the address of its slot: the runtime's non-value as its word, a
 * block given back (its descriptor gone, `hero_array_require`'s test) as `<?>`. */
void hero_show_array(HeroShow *o, const void *p, HeroShowFn elem) {
    const HeroArrayHeader *a = *(HeroArrayHeader *const *)p;
    if (a == NULL) {
        hero_show_text(o, "<unassigned>");
        return;
    }
    if (a->elem == NULL) {
        hero_show_text(o, "<?>");
        return;
    }
    hero_show_elements(o, hero_array_data_const(a), a->len, a->elem->size, elem);
}

/* A fixed array, a member of a group's record: one value inside the record, so
 * it takes its unit of the budget here, where a field's renderer would. */
void hero_show_fixed(HeroShow *o, const void *first, int64_t count, size_t size, HeroShowFn elem) {
    o->values++;
    if (o->values > HERO_SHOW_BUDGET || o->depth > HERO_SHOW_DEPTH) {
        hero_show_text(o, "...");
        return;
    }
    hero_show_elements(o, first, count, size, elem);
}

/* One entry, `k: v`, its key and its value two values inside the map. */
static void hero_show_entry(HeroShow *o, const HeroMapHeader *m, int64_t at, HeroShowFn key, HeroShowFn val) {
    hero_show_child(o, key, hero_map_key_at_const(m, at));
    hero_show_text(o, ": ");
    hero_show_child(o, val, hero_map_val_at_const(m, at));
}

/* Whether text `a` orders before text `b`: by bytes, which is by code point in
 * UTF-8, the shorter first where one begins the other. */
static bool hero_show_before(const char *a, size_t an, const char *b, size_t bn) {
    size_t n = an < bn ? an : bn;
    int c = n > 0 ? memcmp(a, b, n) : 0;
    return c < 0 || (c == 0 && an < bn);
}

typedef struct {
    int64_t at;
    char *bytes;
    size_t used;
} HeroShowKept;

/* `{K: V}`, its entries IN THE ORDER OF THEIR WHOLE TEXT (panel 230's R2, panel
 * 117's map clause), never the order of its buckets, which is the hash's: the
 * text is then a function of the value, with no `cmp` on a record and no sixth
 * descriptor member. Two passes. The first writes every entry on a budget of its
 * own, so its text is the entry's alone, and keeps the SHOWN whose texts come
 * first, a text later than all of them giving way; the second writes those
 * again, in order, on the side's own budget. Two entries whose texts are equal
 * are interchangeable, the panel's *two equal texts cannot show their order*, so
 * which of them the first pass kept moves no byte of the second. */
void hero_show_map(HeroShow *o, const void *p, HeroShowFn key, HeroShowFn val) {
    const HeroMapHeader *m = *(HeroMapHeader *const *)p;
    if (m == NULL) {
        hero_show_text(o, "<unassigned>");
        return;
    }
    if (m->key == NULL || m->val == NULL) {
        hero_show_text(o, "<?>");
        return;
    }
    hero_show_text(o, "{");
    hero_show_open(o);
    HeroShowKept kept[HERO_SHOW_SHOWN];
    size_t held = 0;
    const unsigned char *states = hero_map_states_const(m);
    for (int64_t at = 0; at < m->cap; at++) {
        if (states[at] != 1) continue;
        HeroShow own = {NULL, 0, 0, 0, o->depth};
        hero_show_entry(&own, m, at, key, val);
        if (held < HERO_SHOW_SHOWN) {
            kept[held++] = (HeroShowKept){at, own.bytes, own.used};
            continue;
        }
        size_t last = 0;
        for (size_t k = 1; k < held; k++) {
            if (hero_show_before(kept[last].bytes, kept[last].used, kept[k].bytes, kept[k].used)) last = k;
        }
        if (hero_show_before(own.bytes, own.used, kept[last].bytes, kept[last].used)) {
            hero_release(kept[last].bytes);
            kept[last] = (HeroShowKept){at, own.bytes, own.used};
        } else {
            hero_release(own.bytes);
        }
    }
    for (size_t k = 1; k < held; k++) {
        HeroShowKept hold = kept[k];
        size_t j = k;
        while (j > 0 && hero_show_before(hold.bytes, hold.used, kept[j - 1].bytes, kept[j - 1].used)) {
            kept[j] = kept[j - 1];
            j--;
        }
        kept[j] = hold;
    }
    for (size_t k = 0; k < held; k++) {
        if (k > 0) hero_show_text(o, ", ");
        hero_show_entry(o, m, kept[k].at, key, val);
        hero_release(kept[k].bytes);
    }
    if (m->len > (int64_t)held) hero_show_more(o, held > 0, m->len - (int64_t)held);
    hero_show_close(o);
    hero_show_text(o, "}");
}

/* `assert`'s failure with both sides (panel 230's R2): the head line and the
 * place first, from the fixed buffer, out before anything allocates; then each
 * side, its type's renderer handed in with the two addresses, never their
 * texts, so a renderer that fails loses the sides and nothing above them. An
 * `==` whose two sides are written alike says why, `alike`: a difference past
 * what is shown, or a `nan`, which no value equals; an ordering or an `!=` that
 * fails has sides alike or not by its own meaning, and says nothing. */
_Noreturn void hero_panic_assert_shown_at(HeroStr text, HeroShowFn show, const void *left, const void *right, int alike,
                                          const char *file, const char *function, int64_t line, int64_t column) {
    hero_str_require(text);
    fflush(stdout);
    HeroReport r = {.used = 0};
    HERO_REPORT_OWN(&r, "assert failed: ");
    hero_report_message(&r, text);
    HERO_REPORT_OWN(&r, "\n");
    hero_report_place(&r, file, function, line, column);
    hero_report_flush(&r);
    HeroShow l = {NULL, 0, 0, 0, 0};
    HeroShow rt = {NULL, 0, 0, 0, 0};
    show(&l, left);
    show(&rt, right);
    HERO_REPORT_OWN(&r, "  left:  ");
    hero_report_put(&r, l.bytes, l.used);
    HERO_REPORT_OWN(&r, "\n  right: ");
    hero_report_put(&r, rt.bytes, rt.used);
    HERO_REPORT_OWN(&r, "\n");
    if (alike && l.used == rt.used && (l.used == 0 || memcmp(l.bytes, rt.bytes, l.used) == 0)) {
        HERO_REPORT_OWN(&r, "  the two sides are written alike: they differ past what is shown, or hold a nan, "
                            "which equals nothing, not even a nan\n");
    }
    hero_report_flush(&r);
    hero_abort();
}

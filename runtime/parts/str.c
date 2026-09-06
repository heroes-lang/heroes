/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/str.c — `str` and its reference counting.
 *
 * A `str` is a fat pointer PASSED BY VALUE (`heroes_runtime.h` carries the
 * layout argument). `ptr == NULL` is the one non-value, which is what lets a
 * slot be zero-initialised while a read of an unassigned one stays a loud panic.
 *
 * The live-block counter used to live here, because `str` was the first thing
 * that allocated. It moved to `parts/alloc.c` with every other `malloc` in the
 * runtime, which is what §4.20 has asked for since day zero and what this file
 * had been standing in for.
 *
 * design.md §4.20, §4.10, panel 021.
 */

static const struct {
    HeroStrHeader h;
    char b[1];
} hero_empty_block = {{-1, HERO_STR_MAGIC}, ""};

HeroStr hero_str_empty(void) { return (HeroStr){hero_empty_block.b, 0}; }

/* The header sits immediately before the bytes. One helper, one place where the
 * pointer arithmetic lives. */
static HeroStrHeader *hero_str_hdr(HeroStr s) {
    return (HeroStrHeader *)(void *)((char *)(void *)(uintptr_t)s.ptr -
                                     sizeof(HeroStrHeader));
}

/* Every reader goes through this: a NULL ptr is an unassigned or moved-out
 * slot, and reading one is a compiler bug, not an empty string. */
static void hero_str_require(HeroStr s) {
    if (s.ptr == NULL) {
        hero_panic("read of an unassigned str slot — this is a compiler bug, please report it");
    }
}

static HeroStr hero_str_alloc(int64_t len) {
    if (len < 0) hero_panic("negative string length");
    HeroStrHeader *h = hero_alloc_block(sizeof(HeroStrHeader) + (size_t)len + 1);
    /* RELAXED, and it is the third order this file asks for. A plain `= 1` on an
     * `_Atomic` field is a sequentially consistent store, which is correct and
     * pays for an ordering nobody needs: the block is one line old and its
     * address has not left this function, so no other thread can be looking at
     * it. What publishes it is the caller returning the `HeroStr`, and whatever
     * hands that value to another thread is what carries the ordering. The same
     * store, for the same reason, opens `hero_array_new` and `hero_map_new`. */
    atomic_store_explicit(&h->refcount, 1, memory_order_relaxed);
    h->magic = HERO_STR_MAGIC;
    char *b = (char *)(void *)(h + 1);
    b[len] = '\0';
    return (HeroStr){b, len};
}

/* One place where a str block is validated. */
static HeroStrHeader *hero_str_hdr_checked(HeroStr s) {
    HeroStrHeader *h = hero_str_hdr(s);
    if (h->magic != HERO_STR_MAGIC) {
        hero_panic("not a Heroes string block — a str was fabricated from a "
                   "foreign pointer; use hero_str_from_bytes");
    }
    return h;
}

/* THE TWO MEMORY ORDERS, and they are the same pair at all three containers.
 *
 * `incref` is RELAXED: the thread making a new reference already holds one, so
 * the block cannot go away underneath it and there is nothing this increment has
 * to be ordered against. Only the count itself must not be lost, which is what
 * an atomic read-modify-write buys and a `+= 1` does not.
 *
 * `decref` is ACQ_REL, and the release half is the one that is easy to leave
 * out: the thread that drops the last reference has to SEE every write the other
 * holders made before they let go, or it frees a block while another thread's
 * store to it is still in flight. Release on the way down publishes those
 * writes; acquire on the count reaching zero collects them.
 *
 * `atomic_fetch_sub_explicit` answers with the count BEFORE the subtraction, so
 * `== 1` is "this call took it to zero" — the same test the plain `-= 1`
 * followed by `== 0` was making, in one operation instead of two. */
void hero_str_incref(HeroStr s) {
    if (s.ptr == NULL) return;
    HeroStrHeader *h = hero_str_hdr_checked(s);
    /* A static literal's count never moves, so reading it relaxed is enough —
     * and its block is `const` in read-only memory, which is why the assert in
     * the header demands a lock-free 64-bit atomic: a load that took a lock
     * would be a write to a page nobody may write. */
    if (atomic_load_explicit(&h->refcount, memory_order_relaxed) < 0) return;
    atomic_fetch_add_explicit(&h->refcount, 1, memory_order_relaxed);
}

void hero_str_decref(HeroStr s) {
    if (s.ptr == NULL) return; /* the zero-init non-value: a no-op */
    HeroStrHeader *h = hero_str_hdr_checked(s);
    if (atomic_load_explicit(&h->refcount, memory_order_relaxed) < 0) return; /* static literal */
    if (atomic_fetch_sub_explicit(&h->refcount, 1, memory_order_acq_rel) == 1) {
        hero_release_block(h);
    }
}

/* **`repeat(s, n)` — one allocation where a loop makes n** (panel 054; design.md
 * §4.20, CLAUDE.md §12).
 *
 * Measured before it was written: 1 MB built by concatenation is 8.180 s, one
 * million allocations and 500 GB copied; the same string here is 0.000265 s and
 * one allocation. The spec's cost clause was true and had no exit, and this is
 * the exit.
 *
 * **The multiply is the whole risk, and the division is how it is guarded.** Rust
 * shipped `str::repeat` with an unchecked `len * count` as CVE-2018-1000810 — an
 * out-of-bounds write, live from 1.16.0 to 1.29.0 — and this runtime reproduced
 * the class before adding the guard: `4 * (2^62+2)` wraps **positive to 8**, so
 * eight bytes are allocated and 2^64 are copied, giving **exit 138, SIGBUS, with
 * no message and no output**. `hero_str_alloc`'s `len < 0` check catches only the
 * half that wraps negative. Without this line `hero_str_repeat` would be the only
 * primitive in this runtime whose length can wrap positive — `hero_str_concat`
 * guards its add above, `hero_array_new` guards its product.
 *
 * The count arrives as a `u64` because the language refuses to write a negative
 * one: a computed count goes through `to_u64().must()`, which aborts at the
 * subtraction that went negative rather than here. Go's `strings.Repeat` takes a
 * signed count and oh-my-posh #4135 is the shipped crash that follows from it —
 * from a renderer's padding subtraction, which is what `repeat(" ", col)` is. */
HeroStr hero_str_repeat(HeroStr s, uint64_t n) {
    hero_str_require(s);
    /* Either side empty is the empty string, and asking first is what makes the
     * division below safe as well as cheap. */
    if (n == 0 || s.len == 0) return hero_str_empty();
    if (n > (uint64_t)(INT64_MAX / s.len)) hero_panic("string length overflow");
    int64_t len = s.len * (int64_t)n;
    HeroStr r = hero_str_alloc(len);
    char *w = (char *)(void *)(uintptr_t)r.ptr;
    for (uint64_t i = 0; i < n; i++) memcpy(w + (int64_t)i * s.len, s.ptr, (size_t)s.len);
    return r;
}

HeroStr hero_str_concat(HeroStr a, HeroStr b) {
    hero_str_require(a);
    hero_str_require(b);
    if (a.len > INT64_MAX - b.len) hero_panic("string length overflow");
    HeroStr r = hero_str_alloc(a.len + b.len);
    char *w = (char *)(void *)(uintptr_t)r.ptr;
    memcpy(w, a.ptr, (size_t)a.len);
    memcpy(w + a.len, b.ptr, (size_t)b.len);
    return r;
}

bool hero_str_eq(HeroStr a, HeroStr b) {
    hero_str_require(a);
    hero_str_require(b);
    if (a.len != b.len) return false;
    return memcmp(a.ptr, b.ptr, (size_t)a.len) == 0;
}

int64_t hero_str_cmp(HeroStr a, HeroStr b) {
    hero_str_require(a);
    hero_str_require(b);
    int64_t n = a.len < b.len ? a.len : b.len;
    int c = n == 0 ? 0 : memcmp(a.ptr, b.ptr, (size_t)n);
    if (c != 0) return c < 0 ? -1 : 1;
    if (a.len == b.len) return 0;
    return a.len < b.len ? -1 : 1;
}

int64_t hero_str_len(HeroStr s) {
    hero_str_require(s);
    return s.len;
}

int64_t hero_str_byte(HeroStr s, int64_t i) {
    hero_str_require(s);
    if (i < 0 || i >= s.len) hero_panic("string index out of range");
    return (int64_t)(unsigned char)s.ptr[i];
}

/* UTF-8 continuation bytes are 10xxxxxx. A cut at index `at` lands INSIDE a
 * multi-byte sequence exactly when the byte there is one of them — the ends of
 * the string are always boundaries, whatever the bytes are. */
static bool hero_str_boundary(HeroStr s, int64_t at) {
    if (at <= 0 || at >= s.len) return true;
    unsigned char c = (unsigned char)s.ptr[at];
    return c < 0x80 || c > 0xBF;
}

/* design.md:809 — "Slicing that lands mid-sequence is an error" — a mandate this
 * runtime did not implement until panel 027 went looking for it:
 * `slice("caffè", from: 0, to: 5)` exited 0 and printed a corrupt byte.
 *
 * The abort belongs HERE and not at `chars`, which is where the symptom shows.
 * This is the operation that manufactures the broken string, so the message
 * points at the cause one line earlier, and `chars` stays total over whatever
 * bytes exist (panel 027 R4). `s[i]` remains the byte-level escape hatch: a
 * program that means to walk bytes says so, and says it per byte. */
HeroStr hero_str_slice(HeroStr s, int64_t from, int64_t to) {
    hero_str_require(s);
    if (from < 0 || to < from || to > s.len) hero_panic("string slice out of range");
    if (!hero_str_boundary(s, from) || !hero_str_boundary(s, to)) {
        hero_panic("string slice splits a character");
    }
    if (to == from) return hero_str_empty();
    HeroStr r = hero_str_alloc(to - from);
    memcpy((char *)(void *)(uintptr_t)r.ptr, s.ptr + from, (size_t)(to - from));
    return r;
}

void hero_print_str(HeroStr s) {
    hero_str_require(s);
    fwrite(s.ptr, 1, (size_t)s.len, stdout);
}

/* THE ONE PLACE FOREIGN BYTES BECOME A HEROES `str`, and therefore the one place
 * that can promise what every reader of a `str` assumes: **well-formed UTF-8**.
 *
 * `slice` aborts when it would split a character, `chars` walks continuation
 * bytes, and `len` is documented in bytes over a valid encoding — three rules
 * resting on a premise nothing checked. It held only because §4.19's gate refuses
 * `extern` today, so no foreign byte has ever reached here; M-ffi-ladder is the milestone
 * that kills it, and a check that arrives after the first binding arrives too
 * late (2026-08-12, sweep 001 audit S11).
 *
 * It is a loop over the length rather than a promise in a comment, because the
 * cost is paid once at the boundary and the alternative is a corrupt `str` that
 * every later abort blames on the author. */
bool hero_utf8_valid(const char *p, int64_t len) {
    if (p == NULL) return len == 0;
    int64_t i = 0;
    while (i < len) {
        unsigned char c = (unsigned char)p[i];
        int64_t extra;
        unsigned long lowest;
        unsigned long value;
        if (c < 0x80) {
            i += 1;
            continue;
        } else if ((c & 0xE0) == 0xC0) {
            extra = 1; lowest = 0x80; value = c & 0x1FUL;
        } else if ((c & 0xF0) == 0xE0) {
            extra = 2; lowest = 0x800; value = c & 0x0FUL;
        } else if ((c & 0xF8) == 0xF0) {
            extra = 3; lowest = 0x10000; value = c & 0x07UL;
        } else {
            return false; /* a continuation byte or 0xF8..0xFF as a leader */
        }
        if (i + extra >= len) return false; /* truncated at the end of the buffer */
        for (int64_t k = 1; k <= extra; k++) {
            unsigned char n = (unsigned char)p[i + k];
            if ((n & 0xC0) != 0x80) return false;
            value = (value << 6) | (unsigned long)(n & 0x3F);
        }
        /* Overlong encodings, surrogates and past U+10FFFF are all ill-formed. */
        if (value < lowest) return false;
        if (value >= 0xD800 && value <= 0xDFFF) return false;
        if (value > 0x10FFFF) return false;
        i += extra + 1;
    }
    return true;
}

HeroStr hero_str_from_bytes(const char *p, int64_t len) {
    if (p == NULL) hero_panic("hero_str_from_bytes: NULL pointer from C");
    if (len < 0) hero_panic("hero_str_from_bytes: negative length from C");
    if (!hero_utf8_valid(p, len)) hero_panic("hero_str_from_bytes: not well-formed UTF-8");
    if (len == 0) return hero_str_empty();
    HeroStr r = hero_str_alloc(len);
    memcpy((char *)(void *)(uintptr_t)r.ptr, p, (size_t)len);
    return r;
}

/* **The same copy, but the caller decides what a bad pointer means** (panel 089,
   the resolution's item 1). `hero_str_from_cstr` above aborts on a null pointer
   and on bytes that are not UTF-8, which is right for the *program's* own bytes
   and wrong for the *environment's*: a SQLite TEXT column another program wrote,
   a Latin-1 `PATH`, an argument the shell handed over. This is that conversion
   with a status instead of a grave, and the Heroes side turns the status into a
   `str?` — the exact contract `hero_file_read` already has, down to the promise
   that on anything but OK the returned string is empty and owns nothing.

   **It is written BESIDE `hero_str_from_bytes` rather than on top of it, and
   that is a measurement rather than a preference** (panel 089, ffi-pragmatist
   condition 1). The obvious composition — validate, then call `from_bytes` —
   walks the bytes twice, and validation dominates the copy 25:1, so it measured
   **1.96×** the cost of today's conversion on 16 MiB. One walk, then `alloc` and
   `memcpy` directly, measured **0.87×** — cheaper than what it replaces. A
   fallible conversion that made every good binding slower would be a §13 tax on
   the boundary §1.11 says is the only way anything gets into this language.

   `hero_str_from_bytes`'s abort is deliberately NOT weakened: `hero_str_chars`
   depends on it for the language-wide well-formedness invariant (panel 087, and
   panel 089 measured the mechanism — `chars` hands the offending byte straight
   back to `from_bytes` and dies inside it). Nothing here calls it. */
HeroStr hero_str_try_from_cstr(const char *p, int64_t *status) {
    if (p == NULL) {
        *status = HERO_STR_NULL;
        return hero_str_empty();
    }
    size_t n = strlen(p);
    if (n > (size_t)INT64_MAX) hero_panic("string length overflow");
    int64_t len = (int64_t)n;
    if (!hero_utf8_valid(p, len)) {
        *status = HERO_STR_NOT_TEXT;
        return hero_str_empty();
    }
    *status = HERO_STR_OK;
    if (len == 0) return hero_str_empty();
    HeroStr r = hero_str_alloc(len);
    memcpy((char *)(void *)(uintptr_t)r.ptr, p, (size_t)len);
    return r;
}

/* **A `cstr` on its way INTO C, checked** (panel 053; CLAUDE.md §12's robustness
   rule). `hero_str_from_cstr` above guards the path where C's string comes into
   Heroes; this guards the path where it goes straight back out — `strstr(getenv(
   unset), "y")`, where `to_str` is never called and nothing looked at the pointer.
   Measured before the guard existed: `AddressSanitizer: SEGV on unknown address
   0x0` inside libsystem, which is the class §12 says must not happen.

   It is the same obligation CLAUDE.md §7 already places on arithmetic — abort
   rather than reach C's undefined behaviour — and it is written the same way: one
   branch, a named message, and no cost when the pointer is good. */
const char *hero_cstr_nonnull(const char *p) {
    if (p == NULL) hero_panic("a null `cstr` was passed to a C function");
    return p;
}

HeroStr hero_str_from_cstr(const char *p) {
    if (p == NULL) hero_panic("hero_str_from_cstr: NULL pointer from C");
    return hero_str_from_bytes(p, (int64_t)strlen(p));
}

const char *hero_str_cstr(HeroStr s) {
    hero_str_require(s);
    return s.ptr; /* the NUL is already there: §4.20's zero-copy .cstr() */
}

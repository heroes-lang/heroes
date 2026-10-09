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

/* **Whether a str holds a NUL is one bit of its block, kept from its making**
 * (panel 192's R1, defect 245). A `str` holds any UTF-8, U+0000 included: a
 * file read whole, a binding's own C returning bytes with their length, the
 * compiler's read of a source and the harness's `git ls-files -z` listing all
 * hold one by design, and `print` and `write_file` write every byte. What C
 * cannot be handed is such a str as a C string, since C reads one only to its
 * first NUL, so the lend and the lease ask this bit and stop.
 *
 * The bit is the low bit of the header's existing magic word,
 * `HERO_STR_MAGIC_NUL`, so the layout does not move and the lend pays one load
 * and one test, flat in the string's length (about 5 instructions, the
 * ffi-pragmatist's measurement; a scan at every lend was 0.44 a byte). A block
 * this runtime allocates is marked by its constructor, which knows: the copy
 * from bytes asks them once with `memchr`; concatenation, repetition and a join
 * carry their inputs' bits; a slice keeps the bit only when its own bytes still
 * hold the NUL. A static block (a literal, the empty string) has it computed by
 * clang in `HERO_STR_STATIC`. So the bit is set exactly when the bytes hold a
 * zero, and `fixedbugs-245-*` under `tests/golden/run/` pins each constructor. */
static bool hero_str_holds_nul(HeroStr s) {
    return (hero_str_hdr(s)->magic & UINT64_C(1)) != 0;
}
static void hero_str_mark_nul(HeroStr r) {
    hero_str_hdr(r)->magic = HERO_STR_MAGIC_NUL;
}

/* The words for a block whose mark is gone, said by every reader and by every
 * new reference and release (`hero_str_hdr_checked` below says what it sees). */
_Noreturn static void hero_str_mark_lost(void) {
    hero_panic("a str's block has lost its mark: the bytes just before its text "
               "were overwritten, or the block was freed, or it never was a string "
               "block. Three things do this: C writing or freeing memory this program "
               "still holds, which an `extern` mark that is not true of its function "
               "lets happen (`owned`, `consumes`, `lent`); a `HeroStr` built by hand "
               "in C rather than by `hero_str_from_bytes`; or a compiler bug, which is "
               "worth reporting");
}

/* Every reader goes through this: a NULL ptr is an unassigned or moved-out
 * slot, and reading one is a compiler bug, not an empty string.
 *
 * **AND THE MARK, SO A STRING READ AFTER ITS LAST RELEASE STOPS AT THE READ**
 * (defect 462, 2026-10-07). Only a new reference and a release asked the mark,
 * so a read of a block already given back went on with whatever the freed
 * bytes held. Measured on this Mac at `56def9b4`: panel 190's control, this
 * tree's C of a function with two returns with one release written twice,
 * printed five NUL bytes where `p-one` had been, at `-O0` and `-O2`; a
 * 65,536-byte string C released to its last reference answered its length and
 * a slice from the freed block; each stopped only at the program's own
 * release. The runtime writes the mark over itself before a block goes
 * (`hero_str_decref`), so the read finds no mark whatever the allocator did,
 * and both stop at their first read now. A block the allocator has handed to a
 * new string carries a true mark again, and is ASan's, as for a release.
 *
 * INLINED, AND THAT IS A COUNT: the check is a load and a test, and at `-O0`,
 * where the compiler itself runs, a call costs more than either. Measured in
 * instructions retired on the compiler's own `check` of itself, of
 * `examples/interpreter` and of `examples/roman`: +3.0 to +3.4% with the check
 * as a call, +1.2% inlined, at each of the three; a loop of string, array and map
 * traffic +0.8 to +1.0% at `-O0` and +1.8 to +2.0% at `-O2`, flat from 100,000
 * to 1,000,000 rounds. The array's and the map's checks below are inlined for
 * the same reason. */
__attribute__((always_inline)) static inline void hero_str_require(HeroStr s) {
    if (s.ptr == NULL) {
        hero_panic("read of an unassigned str slot — this is a compiler bug, please report it");
    }
    const HeroStrHeader *h = (const HeroStrHeader *)(const void *)(s.ptr - sizeof(HeroStrHeader));
    if ((h->magic | UINT64_C(1)) != HERO_STR_MAGIC_NUL) hero_str_mark_lost();
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

/* One place where a str block is validated.
 *
 * WHAT IT SEES, AND ONLY THAT — defect 076, 2026-09-23. The magic was put here
 * for one cause, a `HeroStr` built by hand in C (the tag's own comment in
 * heroes_runtime.h), and the message named that cause as the only one. It is
 * not: a C function that writes over the eight bytes before a string's text,
 * or frees a block this program still holds and lets it be reused, reaches the
 * same check. Measured — a C writer over a lent `.cstr()` panics here on every
 * run and under ASan, and a false `owned` on a cell C had already freed did so
 * 8 runs in 10 on Darwin and 5 in 5 on Linux x86-64, while the line said *a
 * str was fabricated*. So it states the fact, that the mark is gone, and names
 * the three causes as causes, worst first; panel 173 R1 is the standard. */
static HeroStrHeader *hero_str_hdr_checked(HeroStr s) {
    HeroStrHeader *h = hero_str_hdr(s);
    if ((h->magic | UINT64_C(1)) != HERO_STR_MAGIC_NUL) hero_str_mark_lost();
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
/* **A RELEASED BLOCK IS NEVER READ AS LIVE, whatever the allocator does with
 * it** (defect 314, 2026-10-07). The mark is the check every release and
 * every new reference passes, and it was read in a block already given back:
 * it saw a release after the last only where the allocator had written over
 * the block's first bytes. Darwin's and glibc's do for a small block; the
 * Windows box's did not, and panel 190's control, one string released twice,
 * exited 0 there with the right output; this Mac's allocator does not either
 * for a block of 20,000 bytes or more, and the same release exited 0 here
 * (measured at `dad2da47`, 5,000 bytes named, 20,000 to 4,000,000 not). A
 * count found at 0 was then taken to -1 in freed memory. So the runtime
 * writes the mark over itself before the block goes, and a count of 0 or
 * below is refused by name; what the allocator writes over it afterwards is
 * no mark either. One store a release, and one test a reference that was
 * already a load.
 *
 * What it cannot see is a block the allocator has already handed to a new
 * string, which carries a true mark and a true count again; that is ASan's,
 * which `--sanitize` turns on. And the overwritten mark is "HERODEAD" and not
 * a word a message tells apart, because whether it survives to be read is the
 * allocator's choice (`hero_held_release`'s note on a first draft that tried). */
#define HERO_STR_RELEASED UINT64_C(0x4845524f44454144) /* "HERODEAD" */

_Noreturn static void hero_str_overreleased(void) {
    hero_panic("a str released after its last reference: its count was already 0. "
               "Two things do this: C releasing a string this program still holds, which "
               "an `extern` mark that is not true of its function lets happen (`owned`, "
               "`consumes`); or a compiler bug, which is worth reporting");
}

void hero_str_incref(HeroStr s) {
    if (s.ptr == NULL) return;
    HeroStrHeader *h = hero_str_hdr_checked(s);
    /* A static literal's count never moves, so reading it relaxed is enough —
     * and its block is `const` in read-only memory, which is why the assert in
     * the header demands a lock-free 64-bit atomic: a load that took a lock
     * would be a write to a page nobody may write. A count of -1 is a static
     * literal's (`HERO_STR_STATIC`, the empty block); any other below 1 is a
     * block this runtime no longer holds. */
    int64_t seen = atomic_load_explicit(&h->refcount, memory_order_relaxed);
    if (seen < 1) {
        if (seen == -1) return;
        hero_str_overreleased();
    }
    atomic_fetch_add_explicit(&h->refcount, 1, memory_order_relaxed);
}

void hero_str_decref(HeroStr s) {
    if (s.ptr == NULL) return; /* the zero-init non-value: a no-op */
    HeroStrHeader *h = hero_str_hdr_checked(s);
    int64_t seen = atomic_load_explicit(&h->refcount, memory_order_relaxed);
    if (seen < 1) {
        if (seen == -1) return; /* static literal */
        hero_str_overreleased();
    }
    /* `> 1` on the count BEFORE the subtraction: another holder remains. At 1
     * this call took the last reference. Below 1 two releases raced for one
     * reference, and the second is refused rather than freeing again. */
    int64_t before = atomic_fetch_sub_explicit(&h->refcount, 1, memory_order_acq_rel);
    if (before > 1) return;
    if (before < 1) hero_str_overreleased();
    h->magic = HERO_STR_RELEASED;
    hero_release_block(h);
}

/* The exit's release through the slot (defect 470; `heroes_runtime.h` says
 * why), and the identity a unit's own release is reached through. */
void hero_str_release_at(const HeroStr *slot) { hero_str_decref(*slot); }

void *hero_slot_escape(void *slot) { return slot; }

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
    if (hero_str_holds_nul(s)) hero_str_mark_nul(r);
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
    if (hero_str_holds_nul(a) || hero_str_holds_nul(b)) hero_str_mark_nul(r);
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
    if (hero_str_holds_nul(s) && memchr(r.ptr, 0, (size_t)(to - from)) != NULL) hero_str_mark_nul(r);
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
 * every later abort blames on the author.
 *
 * **The judge of one sequence is `hero_utf8_sequence`, and it is the only one**
 * (panel 189, defect 227). The compiler's shown read (`hero_file_read_shown`,
 * parts/os.c) has to say WHICH bytes are not text where this says only whether
 * any is, and two validators can silently disagree (panel 035's item 4 named
 * that hazard when this function was exported), so both ask the same function
 * of each byte that is not ASCII. The ASCII byte stays inline here: it is every
 * byte of every file the compiler reads of its own, and the loop for it is the
 * one it always was. */
static int64_t hero_utf8_sequence(const char *p, int64_t i, int64_t len) {
    unsigned char c = (unsigned char)p[i];
    int64_t extra;
    unsigned long lowest;
    unsigned long value;
    if (c < 0x80) {
        return 1;
    } else if ((c & 0xE0) == 0xC0) {
        extra = 1; lowest = 0x80; value = c & 0x1FUL;
    } else if ((c & 0xF0) == 0xE0) {
        extra = 2; lowest = 0x800; value = c & 0x0FUL;
    } else if ((c & 0xF8) == 0xF0) {
        extra = 3; lowest = 0x10000; value = c & 0x07UL;
    } else {
        return 0; /* a continuation byte or 0xF8..0xFF as a leader */
    }
    if (i + extra >= len) return 0; /* truncated at the end of the buffer */
    for (int64_t k = 1; k <= extra; k++) {
        unsigned char n = (unsigned char)p[i + k];
        if ((n & 0xC0) != 0x80) return 0;
        value = (value << 6) | (unsigned long)(n & 0x3F);
    }
    /* Overlong encodings, surrogates and past U+10FFFF are all ill-formed. */
    if (value < lowest) return 0;
    if (value >= 0xD800 && value <= 0xDFFF) return 0;
    if (value > 0x10FFFF) return 0;
    return extra + 1;
}

bool hero_utf8_valid(const char *p, int64_t len) {
    if (p == NULL) return len == 0;
    int64_t i = 0;
    while (i < len) {
        if ((unsigned char)p[i] < 0x80) {
            i += 1;
            continue;
        }
        int64_t n = hero_utf8_sequence(p, i, len);
        if (n == 0) return false;
        i += n;
    }
    return true;
}

HeroStr hero_str_from_bytes(const char *p, int64_t len) {
    if (p == NULL) hero_panic("hero_str_from_bytes: NULL pointer from C");
    if (len < 0) hero_panic("hero_str_from_bytes: negative length from C");
    if (!hero_utf8_valid(p, len)) hero_panic("hero_str_from_bytes: not well-formed UTF-8");
    if (len == 0) return hero_str_empty();
    bool holds_nul = memchr(p, 0, (size_t)len) != NULL;
    HeroStr r = hero_str_alloc(len);
    memcpy((char *)(void *)(uintptr_t)r.ptr, p, (size_t)len);
    if (holds_nul) hero_str_mark_nul(r);
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

/* **A FIXED RUN OF BYTES, whose end the caller states** (panel 162,
   M-readable-bytes). `hero_str_try_from_cstr` above is this function with one
   line different — its line 304 is `size_t n = strlen(p);` — and that one line
   is why it cannot serve here: a C `char[N]` field is not a `cstr` and need not
   be NUL-terminated at all. Panel 162 measured the population: 16 headers, 712
   struct fields, **50** of type `char[N]`, and only **13 reliably
   NUL-terminated**. A `strlen` over the other 37 walks past the field.

   **So the rule is: to its first zero, or whole.** `memchr` over exactly `cap`
   bytes, and where there is no zero the whole field is the text. That phrasing
   is the spec-warden's and it is load-bearing — a terminator-only rule
   over-reads three-quarters of the fields this exists for, and unlike a null
   handle there is no runtime guard to catch it, because the pointer is valid.

   Everything else is `hero_str_try_from_cstr`'s, deliberately: the same status
   word, the same one-walk discipline (validate, then `alloc` and `memcpy`, never
   validate-then-reconvert, which panel 089 measured at 1.96× against 0.87×), and
   the same promise that on anything but OK the returned string is empty and owns
   nothing. `read_file` answers `not_text` on bad UTF-8 and so does this; a
   sitting that invented a different answer would have been spending tokens on a
   contradiction. */
/* The one walk both readers of a run of bytes share: `len` bytes judged as
 * UTF-8, then copied, the block marked when they hold a NUL. */
static HeroStr hero_str_try_from_run(const char *p, int64_t len, bool may_hold_nul,
                                     int64_t *status) {
    if (!hero_utf8_valid(p, len)) {
        *status = HERO_STR_NOT_TEXT;
        return hero_str_empty();
    }
    *status = HERO_STR_OK;
    if (len == 0) return hero_str_empty();
    HeroStr r = hero_str_alloc(len);
    memcpy((char *)(void *)(uintptr_t)r.ptr, p, (size_t)len);
    if (may_hold_nul && memchr(p, 0, (size_t)len) != NULL) hero_str_mark_nul(r);
    return r;
}

HeroStr hero_str_try_from_bytes(const char *p, int64_t cap, int64_t *status) {
    if (p == NULL) {
        *status = HERO_STR_NULL;
        return hero_str_empty();
    }
    if (cap < 0) hero_panic("hero_str_try_from_bytes: negative capacity");
    const void *zero = memchr(p, 0, (size_t)cap);
    int64_t len = (zero == NULL) ? cap : (int64_t)((const char *)zero - p);
    return hero_str_try_from_run(p, len, false, status);
}

/* **Over a `[u8]` this language owns, the WHOLE array** (panel 162; defect
   354, 2026-10-05). It exists rather than the emitter composing
   `hero_array_at` with a reader of bytes, for one measured reason:
   `hero_array_at` ABORTS out of range, so the composition needs a length guard
   at every call site and an empty array is exactly the shape that trips it.
   One function, one guard, written once.

   **It does not stop at a zero, where the field's reader above does.** A C
   field is C's, and C need not terminate it, so what follows its first zero
   is not text. A `[u8]` is the program's own data, every byte of it put there
   on purpose, and under panel 192's R1 a `str` holds a NUL: until defect 354,
   `[97, 0, 98]` answered a `str` of one byte at exit 0, and a byte that is not
   text after the zero was never judged. So every byte is judged and copied,
   and a zero among them marks the block, so the lend stops it.

   The element width is not checked here and that is the checker's job, not
   this one's: `check/lending.hero`'s `byte_run` asks the VALUE's element width
   and admits 8 bits only, so a `[i64]` never reaches this line. */
HeroStr hero_str_try_from_array(const HeroArrayHeader *a, int64_t *status) {
    if (a == NULL) {
        *status = HERO_STR_NULL;
        return hero_str_empty();
    }
    int64_t n = hero_array_len(a);
    if (n == 0) {
        *status = HERO_STR_OK;
        return hero_str_empty();
    }
    return hero_str_try_from_run((const char *)hero_array_at(a, 0), n, true, status);
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

/* **The lend a program writes, `.cstr()`** (panel 192's R1, defect 245). The
 * same zero-copy pointer `hero_str_cstr` above answers, refused where the
 * str's block says it holds a NUL: C would read a shorter string than the
 * program holds, and a path cut at its NUL names ANOTHER file (the
 * ffi-pragmatist's probes: on the base, 19 of 29 doors changed, made, read or
 * ran what the program never named). An abort, as for a null `cstr`
 * (`hero_cstr_nonnull`) and for `hero_run_arg`'s word: the door is a binding,
 * and a value C cannot be handed is named rather than handed over cut. The
 * two doors whose types promise a failure, `read_file` and `write_file`, take
 * the `str` itself and answer one (`hero_file_read_str`, `parts/os.c`).
 *
 * `hero_str_cstr` stays the runtime's own lend: its callers pass the length
 * beside the pointer (`hero_file_write`, the replace doors), so a NUL is data
 * there and refusing it would make `write_file` fail a correct program.
 *
 * The byte is found only on the way to the abort, for the message; its index
 * is the one `s[i]` reads. */
static _Noreturn void hero_str_refuse_nul(HeroStr s, const char *what) {
    const char *zero = memchr(s.ptr, 0, (size_t)s.len);
    char message[320];
    snprintf(message, sizeof message,
             "%s a str holding a NUL byte, at index %lld of its %lld bytes: C reads a "
             "string only to its first NUL, so it would read a shorter string than the "
             "program holds",
             what, (long long)(zero == NULL ? -1 : zero - s.ptr), (long long)s.len);
    hero_panic(message);
}

const char *hero_str_lend(HeroStr s) {
    hero_str_require(s);
    if (hero_str_holds_nul(s)) hero_str_refuse_nul(s, "`.cstr()` lends to C");
    return s.ptr;
}

/* -- the held buffer: §4.19's fourth case, panel 124 -------------------------
 *
 * `s.lease()` answers a COPY of the bytes that the program owns and frees, so C
 * may read them after the call that took them. `.cstr()` above is the lend: the
 * same pointer as the `str`'s, zero copies, alive as long as its owner slot is.
 * This is the other thing entirely, and the two must not be confused, which is
 * why this one carries its own header and its own magic word.
 *
 * WHY A COPY. Handing out `s.ptr` and calling it held would make one allocation
 * with two owners: a copy-on-write mutation through the `str` unshares and
 * rewrites, and C would be reading a block the language believes it owns alone.
 * §4.20 says a str's bytes "may be shared with other values", which is the
 * sentence `ffi_writable_parameter` already cites, so the pin is a corruption
 * class and panel 124 R3 refuses it before it can be written. */
const char *hero_str_held(HeroStr s) {
    hero_str_require(s);
    if (hero_str_holds_nul(s)) hero_str_refuse_nul(s, "`.lease()` copies for C");
    HeroHeldHeader *h = hero_alloc_held(sizeof(HeroHeldHeader) + (size_t)s.len + 1);
    h->magic = HERO_HELD_MAGIC;
    h->len = s.len;
    char *b = (char *)(void *)(h + 1);
    if (s.len > 0) memcpy(b, s.ptr, (size_t)s.len);
    b[s.len] = '\0'; /* §4.20's NUL, kept by the runtime rather than by a caller */
    return b;
}

/* The release the program writes, and it takes the CELL and not the pointer.
 *
 * WHY THE CELL. Panel 125's two seats measured the same thing from two sides: a
 * guard that validates a pointer by reading a magic word BEFORE it is undefined
 * behaviour on three of the four inputs its message would name — a C literal,
 * a `malloc` from C and a stack buffer all read memory this runtime was never
 * handed (global-, heap- and stack-buffer overflow under ASan) — and a double
 * release cannot be told from a never-held pointer by any such read, because
 * the block is gone. So the discriminator is not in the runtime at all. The
 * COMPILER admits `end_lease` only on a cell whose initialiser is a `.lease()`
 * call and refuses every other write to it (`selfhost/check/lending.hero`),
 * and this function nulls the cell on the way out. In an accepted program the
 * pointer here is therefore either NULL, which is the second release and is
 * caught before any read, or a live block this runtime made — in-bounds by
 * construction.
 *
 * TWO refusals, not three: a null cell, and a block whose mark is gone. A first
 * draft claimed a third, "already released", from a zeroed magic in freed
 * memory; five runs printed the wrong message five times, because freed memory
 * owes nobody its contents.
 *
 * AND THE SECOND REFUSAL IS REACHED BY A PROGRAM, not only by an emitter bug —
 * defect 076, 2026-09-23. This comment said the magic check was *"never the
 * guard against a program"* and the message blamed the compiler. A C function
 * handed the lease that writes over the sixteen bytes before its pointer
 * reaches it, deterministically, on every run and under ASan too, because the
 * write stays inside this runtime's own block. So the line says what was SEEN,
 * that the mark is gone, and names the causes as causes; panel 173 R1's rule,
 * *no path prints a sentence measured false*, is the standard. */
void hero_held_release(const char **slot) {
    if (slot == NULL) {
        hero_panic("release with no cell — this is a compiler bug, please report it");
    }
    const char *p = *slot;
    if (p == NULL) {
        hero_panic("end_lease of a lease that is already over — every `.lease()` "
                   "owes exactly one `end_lease`, and this cell has had its one");
    }
    HeroHeldHeader *h =
        (HeroHeldHeader *)(void *)((char *)(void *)(uintptr_t)p - sizeof(HeroHeldHeader));
    if (h->magic != HERO_HELD_MAGIC) {
        hero_panic("end_lease found a lease whose block has lost its mark: the bytes "
                   "just before what C was lent were overwritten, or the block was freed. "
                   "Two things do this: a C function that writes before the pointer it "
                   "is handed, or frees what it is handed, which no word can declare; or "
                   "a compiler bug, which is worth reporting");
    }
    h->magic = 0;
    *slot = NULL;
    hero_release_held(h);
}

/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION): a program
 * compiled with Heroes carries part of this runtime inside it and owes nothing
 * for doing so. The exception is stated here in prose rather than after a
 * `WITH` in the tag above, because that operator takes an exception from
 * SPDX's own registry and this one is not in it. */

/* parts/str.c — `str`, its reference counting, and the live-block counter.
 *
 * A `str` is a fat pointer PASSED BY VALUE (`heroes_runtime.h` carries the
 * layout argument). `ptr == NULL` is the one non-value, which is what lets a
 * slot be zero-initialised while a read of an unassigned one stays a loud panic.
 *
 * `hero_live_blocks` lives here rather than in `panic.c` because `str` is the
 * first thing that allocates. It is `static`, and that is the leak gate's
 * foundation: AddressSanitizer has no leak detector on Darwin arm64 (panel 021,
 * measured), so this counter is the instrument, and nothing outside this
 * translation unit can touch it.
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

/* live heap-block balance: the leak detector that works on this platform */
static int64_t hero_live_blocks = 0;
int64_t hero_runtime_live(void) { return hero_live_blocks; }
void hero_runtime_check_leaks(void) {
    if (hero_live_blocks != 0) {
        fflush(stdout);
        fprintf(stderr, "panic: %lld heap blocks still live at exit "
                        "(a missing decref) — this is a compiler bug\n",
                (long long)hero_live_blocks);
        abort();
    }
}

static HeroStr hero_str_alloc(int64_t len) {
    if (len < 0) hero_panic("negative string length");
    HeroStrHeader *h = malloc(sizeof(HeroStrHeader) + (size_t)len + 1);
    if (h == NULL) hero_panic("out of memory");
    h->refcount = 1;
    h->magic = HERO_STR_MAGIC;
    hero_live_blocks += 1;
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

void hero_str_incref(HeroStr s) {
    if (s.ptr == NULL) return;
    HeroStrHeader *h = hero_str_hdr_checked(s);
    if (h->refcount < 0) return; /* static literal */
    h->refcount += 1;
}

void hero_str_decref(HeroStr s) {
    if (s.ptr == NULL) return; /* the zero-init non-value: a no-op */
    HeroStrHeader *h = hero_str_hdr_checked(s);
    if (h->refcount < 0) return; /* static literal */
    h->refcount -= 1;
    if (h->refcount == 0) {
        hero_live_blocks -= 1;
        free(h);
    }
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

HeroStr hero_str_from_bytes(const char *p, int64_t len) {
    if (p == NULL) hero_panic("hero_str_from_bytes: NULL pointer from C");
    if (len == 0) return hero_str_empty();
    HeroStr r = hero_str_alloc(len);
    memcpy((char *)(void *)(uintptr_t)r.ptr, p, (size_t)len);
    return r;
}

HeroStr hero_str_from_cstr(const char *p) {
    if (p == NULL) hero_panic("hero_str_from_cstr: NULL pointer from C");
    return hero_str_from_bytes(p, (int64_t)strlen(p));
}

const char *hero_str_cstr(HeroStr s) {
    hero_str_require(s);
    return s.ptr; /* the NUL is already there: §4.20's zero-copy .cstr() */
}

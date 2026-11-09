/* Defect 2383: three records whose named bit-fields the MSVC target lays
 * out in other bytes than the Itanium layout does, Darwin's and glibc's. C
 * returns each with every byte set, then its members; the program stores it
 * in a field of its own record and lends it; C compares the bytes it
 * receives with the same members built from zero bytes: 0 where every byte
 * agrees, else 1 plus the first that does not. The reference is built by the
 * same C, so every layout gives one line, and the C the compiler writes for
 * the program is one text on every target: no byte of a bit-field in it. */
#include <stdint.h>
#include <string.h>

/* `b` straddles bytes 0 and 1 after `a`'s seven bits under the Itanium
 * layout; the MSVC one starts a unit of `b`'s own type for it at byte 4, so
 * it reaches one byte there and two here. */
struct fixedbugs_2383_across { uint8_t a : 7; unsigned b : 3; int8_t c; };

/* `s` and `f` share byte 1 under the Itanium layout; the MSVC one gives
 * each a unit of its own type, at bytes 4 and 8. */
struct fixedbugs_2383_apart { int8_t lo; signed s : 3; _Bool f : 1; int16_t h; };

/* Forty bits after a byte: from bit 72 under the Itanium layout, the record
 * 16 bytes; from byte 16 under the MSVC one, its unit eight bytes wide, the
 * record 24. */
struct fixedbugs_2383_wide { int64_t x; uint8_t a; unsigned long long b : 40; };

static inline void fixedbugs_2383_across_set(struct fixedbugs_2383_across *v) { v->a = 85; v->b = 5; v->c = 9; }
static inline void fixedbugs_2383_apart_set(struct fixedbugs_2383_apart *v) { v->lo = 3; v->s = -2; v->f = 1; v->h = 300; }
static inline void fixedbugs_2383_wide_set(struct fixedbugs_2383_wide *v) { v->x = 7; v->a = 2; v->b = 0x5A5A5A5A5AULL; }

static inline struct fixedbugs_2383_across fixedbugs_2383_mk_across(void) { struct fixedbugs_2383_across v; memset(&v, 0xFF, sizeof v); fixedbugs_2383_across_set(&v); return v; }
static inline struct fixedbugs_2383_apart fixedbugs_2383_mk_apart(void) { struct fixedbugs_2383_apart v; memset(&v, 0xFF, sizeof v); fixedbugs_2383_apart_set(&v); return v; }
static inline struct fixedbugs_2383_wide fixedbugs_2383_mk_wide(void) { struct fixedbugs_2383_wide v; memset(&v, 0xFF, sizeof v); fixedbugs_2383_wide_set(&v); return v; }

/* 0 where every byte equals the reference's, else 1 plus the first that does not. */
static inline int64_t fixedbugs_2383_diff(const void *got, const void *want, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (((const unsigned char *)got)[i] != ((const unsigned char *)want)[i]) return (int64_t)i + 1;
    return 0;
}
static inline int64_t fixedbugs_2383_check_across(const struct fixedbugs_2383_across *p) { struct fixedbugs_2383_across r; memset(&r, 0, sizeof r); fixedbugs_2383_across_set(&r); return fixedbugs_2383_diff(p, &r, sizeof r); }
static inline int64_t fixedbugs_2383_check_apart(const struct fixedbugs_2383_apart *p) { struct fixedbugs_2383_apart r; memset(&r, 0, sizeof r); fixedbugs_2383_apart_set(&r); return fixedbugs_2383_diff(p, &r, sizeof r); }
static inline int64_t fixedbugs_2383_check_wide(const struct fixedbugs_2383_wide *p) { struct fixedbugs_2383_wide r; memset(&r, 0, sizeof r); fixedbugs_2383_wide_set(&r); return fixedbugs_2383_diff(p, &r, sizeof r); }

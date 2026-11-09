/* Defect 2385: unions whose arms name bit-fields over the same bits. C
 * returns each record with every byte set, then the member it means; the
 * program stores it in a field of its own record and lends it; C compares
 * the bytes it receives with a reference built from zero bytes in which every
 * named bit-field of every arm is all ones and the member C set is then set,
 * which is what a lend must leave: no bit a named bit-field of any arm holds
 * written, every other bit zero. 0 where every byte agrees, else 1 plus the
 * first that does not, and the member's own value is returned beside it by
 * the `_value` functions. */
#include <stdint.h>
#include <string.h>

/* `x` in bits 0-2 of one arm, `y` in bits 0-4 of the other. */
struct fixedbugs_2385_two { int32_t k; union { struct { unsigned x : 3; unsigned : 5; } p; struct { unsigned y : 5; unsigned : 3; } q; } u; };

/* `a` in bits 0-1, `b` in bits 1-4, `c` in bits 0-6. */
struct fixedbugs_2385_three { int32_t k; union { struct { unsigned a : 2; unsigned : 6; } p; struct { unsigned : 1; unsigned b : 4; unsigned : 3; } q; struct { unsigned c : 7; unsigned : 1; } r; } u; };

/* `y` straddles bytes 0 and 1 of its arm, over `x`'s bits 4 and 5. */
struct fixedbugs_2385_arms { union { struct { unsigned x : 6; unsigned : 2; } p; struct { unsigned : 4; unsigned y : 8; unsigned : 4; } q; } u; };
struct fixedbugs_2385_straddle { int32_t k; struct fixedbugs_2385_arms s; };

/* The same union, a member of a record the bound record holds. */
struct fixedbugs_2385_nested { int64_t h; struct fixedbugs_2385_straddle in; };

static inline void fixedbugs_2385_two_ones(struct fixedbugs_2385_two *v) { v->u.p.x = 7; v->u.q.y = 31; }
static inline void fixedbugs_2385_two_set(struct fixedbugs_2385_two *v) { v->k = 1; v->u.q.y = 22; }
static inline void fixedbugs_2385_three_ones(struct fixedbugs_2385_three *v) { v->u.p.a = 3; v->u.q.b = 15; v->u.r.c = 127; }
static inline void fixedbugs_2385_three_set(struct fixedbugs_2385_three *v) { v->k = 2; v->u.r.c = 85; }
static inline void fixedbugs_2385_straddle_ones(struct fixedbugs_2385_straddle *v) { v->s.u.p.x = 63; v->s.u.q.y = 255; }
static inline void fixedbugs_2385_straddle_set(struct fixedbugs_2385_straddle *v) { v->k = 3; v->s.u.q.y = 165; }
static inline void fixedbugs_2385_nested_ones(struct fixedbugs_2385_nested *v) { fixedbugs_2385_straddle_ones(&v->in); }
static inline void fixedbugs_2385_nested_set(struct fixedbugs_2385_nested *v) { v->h = 4; fixedbugs_2385_straddle_set(&v->in); }

static inline struct fixedbugs_2385_two fixedbugs_2385_mk_two(void) { struct fixedbugs_2385_two v; memset(&v, 0xFF, sizeof v); fixedbugs_2385_two_set(&v); return v; }
static inline struct fixedbugs_2385_three fixedbugs_2385_mk_three(void) { struct fixedbugs_2385_three v; memset(&v, 0xFF, sizeof v); fixedbugs_2385_three_set(&v); return v; }
static inline struct fixedbugs_2385_straddle fixedbugs_2385_mk_straddle(void) { struct fixedbugs_2385_straddle v; memset(&v, 0xFF, sizeof v); fixedbugs_2385_straddle_set(&v); return v; }
static inline struct fixedbugs_2385_nested fixedbugs_2385_mk_nested(void) { struct fixedbugs_2385_nested v; memset(&v, 0xFF, sizeof v); fixedbugs_2385_nested_set(&v); return v; }

/* 0 where every byte equals the reference's, else 1 plus the first that does not. */
static inline int64_t fixedbugs_2385_diff(const void *got, const void *want, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (((const unsigned char *)got)[i] != ((const unsigned char *)want)[i]) return (int64_t)i + 1;
    return 0;
}
static inline int64_t fixedbugs_2385_check_two(const struct fixedbugs_2385_two *p) { struct fixedbugs_2385_two r; memset(&r, 0, sizeof r); fixedbugs_2385_two_ones(&r); fixedbugs_2385_two_set(&r); return fixedbugs_2385_diff(p, &r, sizeof r); }
static inline int64_t fixedbugs_2385_check_three(const struct fixedbugs_2385_three *p) { struct fixedbugs_2385_three r; memset(&r, 0, sizeof r); fixedbugs_2385_three_ones(&r); fixedbugs_2385_three_set(&r); return fixedbugs_2385_diff(p, &r, sizeof r); }
static inline int64_t fixedbugs_2385_check_straddle(const struct fixedbugs_2385_straddle *p) { struct fixedbugs_2385_straddle r; memset(&r, 0, sizeof r); fixedbugs_2385_straddle_ones(&r); fixedbugs_2385_straddle_set(&r); return fixedbugs_2385_diff(p, &r, sizeof r); }
static inline int64_t fixedbugs_2385_check_nested(const struct fixedbugs_2385_nested *p) { struct fixedbugs_2385_nested r; memset(&r, 0, sizeof r); fixedbugs_2385_nested_ones(&r); fixedbugs_2385_nested_set(&r); return fixedbugs_2385_diff(p, &r, sizeof r); }

static inline int64_t fixedbugs_2385_two_value(const struct fixedbugs_2385_two *p) { return (int64_t)p->u.q.y; }
static inline int64_t fixedbugs_2385_three_value(const struct fixedbugs_2385_three *p) { return (int64_t)p->u.r.c; }
static inline int64_t fixedbugs_2385_straddle_value(const struct fixedbugs_2385_straddle *p) { return (int64_t)p->s.u.q.y; }
static inline int64_t fixedbugs_2385_nested_value(const struct fixedbugs_2385_nested *p) { return (int64_t)p->in.s.u.q.y; }

/* Panel 223 (defect 2099): records C returns by value with every bit of
   every byte first set, then its members; each is held in a Heroes record's
   field and lent to C, which compares what it receives with the same record
   built from zero bytes and the same members. Every bit no named bit-field
   or member holds must be zero, and every member kept: the bits beside a
   named bit-field, a byte only an unnamed bit-field holds, a bit-field across
   a byte, signed and `_Bool` ones, one nested in a record and in an array,
   and a union whose byte arm overlaps an arm of bit-fields, whose bytes are
   the arm's and stay as C wrote them. Layout-independent: the reference is
   built by the same C, so the MSVC layout and the others give one line. */
#include <stdint.h>
#include <string.h>

struct bf { char a; unsigned b : 3; unsigned : 2; unsigned c : 4; double d; };
struct ux { int32_t k; union { struct { unsigned x : 3; unsigned : 5; unsigned w : 4; }; unsigned char y[2]; }; };
struct gp { unsigned x : 1; unsigned : 20; unsigned y : 1; char z; };
struct md { unsigned char lo; unsigned mid : 10; signed s : 3; _Bool f : 1; int64_t n; };
struct ob { int32_t k; struct bf inr; };
struct ar { struct bf e[2]; int32_t n; };

static inline void bf_set(struct bf *v, int c) { v->a = 9; v->b = 5; v->c = (unsigned)c; v->d = 3.0; }
static inline void ux_set(struct ux *v) { v->k = 7; v->y[0] = 0xAB; v->y[1] = 0xCD; }
static inline void gp_set(struct gp *v) { v->x = 1; v->y = 0; v->z = 4; }
static inline void md_set(struct md *v) { v->lo = 1; v->mid = 0x2A5; v->s = -2; v->f = 0; v->n = 5; }

static inline struct bf mk_bf(void) { struct bf v; memset(&v, 0xFF, sizeof v); bf_set(&v, 10); return v; }
static inline struct ux mk_ux(void) { struct ux v; memset(&v, 0xFF, sizeof v); ux_set(&v); return v; }
static inline struct gp mk_gp(void) { struct gp v; memset(&v, 0xFF, sizeof v); gp_set(&v); return v; }
static inline struct md mk_md(void) { struct md v; memset(&v, 0xFF, sizeof v); md_set(&v); return v; }
static inline struct ob mk_ob(void) { struct ob v; memset(&v, 0xFF, sizeof v); v.k = 3; v.inr = mk_bf(); return v; }
static inline struct ar mk_ar(void) { struct ar v; memset(&v, 0xFF, sizeof v); v.e[0] = mk_bf(); v.e[1] = mk_bf(); v.e[1].c = 3; v.n = 2; return v; }

/* 0 where every byte equals the reference's, else 1 plus the first that does not. */
static inline int64_t bf_diff(const void *got, const void *want, size_t n) {
    for (size_t i = 0; i < n; i++)
        if (((const unsigned char *)got)[i] != ((const unsigned char *)want)[i]) return (int64_t)i + 1;
    return 0;
}
static inline int64_t check_bf(const struct bf *p) { struct bf r; memset(&r, 0, sizeof r); bf_set(&r, 10); return bf_diff(p, &r, sizeof r); }
static inline int64_t check_ux(const struct ux *p) { struct ux r; memset(&r, 0, sizeof r); ux_set(&r); return bf_diff(p, &r, sizeof r); }
static inline int64_t check_gp(const struct gp *p) { struct gp r; memset(&r, 0, sizeof r); gp_set(&r); return bf_diff(p, &r, sizeof r); }
static inline int64_t check_md(const struct md *p) { struct md r; memset(&r, 0, sizeof r); md_set(&r); return bf_diff(p, &r, sizeof r); }
static inline int64_t check_ob(const struct ob *p) { struct ob r; memset(&r, 0, sizeof r); r.k = 3; bf_set(&r.inr, 10); return bf_diff(p, &r, sizeof r); }
static inline int64_t check_ar(const struct ar *p) { struct ar r; memset(&r, 0, sizeof r); bf_set(&r.e[0], 10); bf_set(&r.e[1], 3); r.n = 2; return bf_diff(p, &r, sizeof r); }

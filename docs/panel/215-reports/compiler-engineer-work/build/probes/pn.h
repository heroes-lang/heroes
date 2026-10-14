/* Compiler-engineer, panel 215 build: the shapes beside the critic's. Each
   mk_* returns a record by value from a local whose MEMBERS alone C wrote,
   so its padding is uninitialised in C's own value; each w_* hands every byte
   it is lent to write(2) on /dev/null, which MemorySanitizer checks. */
#ifndef PN_H
#define PN_H
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
struct cd { char c; double d; };                                   /* 16: 1..7 */
struct arr { struct cd a[2]; int32_t n; };                         /* 40: 1..7, 17..23, 36..39 */
struct ocd2 { struct cd inr; char z; };                            /* 24: 1..7, 17..23 */
struct deep { char k; struct ocd2 o; };                            /* 32: 1..7, 9..15, 25..31 */
struct an { char k; struct { char x; int64_t y; }; };              /* 24: 1..7, 9..15 */
struct bf { char a; unsigned b : 3; unsigned : 2; unsigned c : 4; double d; }; /* bits of 1..2, 3..7 */
struct pt { char k; double d; int32_t extra; char tail[3]; };      /* 24: 1..7, 23 */
struct uk { int32_t kind; union { struct cd s; char one; }; };     /* 24: 4..7, 9..15 padding in both arms */
static inline int pn_null(void) { static int fd = -1; if (fd < 0) fd = open("/dev/null", O_WRONLY); return fd; }
#define PN_W(T) static inline long w_##T(const struct T *p) { return (long)write(pn_null(), p, sizeof *p); }
PN_W(arr) PN_W(deep) PN_W(an) PN_W(bf) PN_W(pt) PN_W(uk)
static inline struct cd pn_cd(void) { struct cd v; v.c = 1; v.d = 2.0; return v; }
static inline struct arr mk_arr(void) { struct arr v; v.a[0] = pn_cd(); v.a[1] = pn_cd(); v.n = 3; return v; }
static inline struct deep mk_deep(void) { struct deep v; v.k = 4; v.o.inr = pn_cd(); v.o.z = 5; return v; }
static inline struct an mk_an(void) { struct an v; v.k = 6; v.x = 7; v.y = 8; return v; }
static inline struct bf mk_bf(void) { struct bf v; v.a = 9; v.b = 1; v.c = 2; v.d = 3.0; return v; }
static inline struct pt mk_pt(void) { struct pt v; v.k = 10; v.d = 4.0; v.extra = 11; memcpy(v.tail, "ab", 3); return v; }
static inline struct uk mk_uk(void) { struct uk v; v.kind = 12; v.s = pn_cd(); return v; }
static inline double uk_d(const struct uk *p) { return p->s.d; }
#endif

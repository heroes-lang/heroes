/* Defect 2346: panel 233's chain of group records by named halves, ten levels
 * and 1,024 leaves, each leaf a member of eight bytes and one of four, so four
 * bytes of padding follow each. C builds the record with every byte set and
 * then its members, the caller lends it, and C compares what it receives with
 * the same record built from zero bytes: 0 where every byte agrees, else 1
 * plus the first that does not. The levels are two halves each with nothing
 * between them on every target, so the leaves are read as one array. */
#include <stdint.h>
#include <string.h>

typedef struct fixedbugs_2346_p0 { int64_t a; int32_t b; } fixedbugs_2346_p0;
typedef struct fixedbugs_2346_p1 { fixedbugs_2346_p0 left; fixedbugs_2346_p0 right; } fixedbugs_2346_p1;
typedef struct fixedbugs_2346_p2 { fixedbugs_2346_p1 left; fixedbugs_2346_p1 right; } fixedbugs_2346_p2;
typedef struct fixedbugs_2346_p3 { fixedbugs_2346_p2 left; fixedbugs_2346_p2 right; } fixedbugs_2346_p3;
typedef struct fixedbugs_2346_p4 { fixedbugs_2346_p3 left; fixedbugs_2346_p3 right; } fixedbugs_2346_p4;
typedef struct fixedbugs_2346_p5 { fixedbugs_2346_p4 left; fixedbugs_2346_p4 right; } fixedbugs_2346_p5;
typedef struct fixedbugs_2346_p6 { fixedbugs_2346_p5 left; fixedbugs_2346_p5 right; } fixedbugs_2346_p6;
typedef struct fixedbugs_2346_p7 { fixedbugs_2346_p6 left; fixedbugs_2346_p6 right; } fixedbugs_2346_p7;
typedef struct fixedbugs_2346_p8 { fixedbugs_2346_p7 left; fixedbugs_2346_p7 right; } fixedbugs_2346_p8;
typedef struct fixedbugs_2346_p9 { fixedbugs_2346_p8 left; fixedbugs_2346_p8 right; } fixedbugs_2346_p9;
typedef struct fixedbugs_2346_p10 { fixedbugs_2346_p9 left; fixedbugs_2346_p9 right; } fixedbugs_2346_p10;

static inline void fixedbugs_2346_p_set(fixedbugs_2346_p10 *v) {
    fixedbugs_2346_p0 *leaf = (fixedbugs_2346_p0 *)(void *)v;
    for (size_t i = 0; i < sizeof *v / sizeof *leaf; i++) {
        leaf[i].a = (int64_t)i;
        leaf[i].b = (int32_t)(3 * i);
    }
}

static inline fixedbugs_2346_p10 fixedbugs_2346_p_make(void) {
    fixedbugs_2346_p10 v;
    memset(&v, 0xFF, sizeof v);
    fixedbugs_2346_p_set(&v);
    return v;
}

static inline int64_t fixedbugs_2346_p_check(const fixedbugs_2346_p10 *p) {
    static fixedbugs_2346_p10 r;
    memset(&r, 0, sizeof r);
    fixedbugs_2346_p_set(&r);
    for (size_t i = 0; i < sizeof r; i++)
        if (((const unsigned char *)p)[i] != ((const unsigned char *)&r)[i]) return (int64_t)i + 1;
    return 0;
}

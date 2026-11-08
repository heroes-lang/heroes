/* Defect 2346: panel 233's chain of group records by named halves, ten levels
 * and 1,024 leaves, each leaf a member of eight bytes and a named bit-field of
 * three bits, so the five bits beside it in its byte and the bytes after it
 * hold nothing. The leaf is bound `partial`, so its members come from the
 * header's layout dump, the bit-field among them. C builds the record with
 * every byte set and then its members, the caller lends it, and C compares
 * what it receives with the same record built from zero bytes: 0 where every
 * byte agrees, else 1 plus the first that does not. The reference is built by
 * the same C, so every layout gives the one line. */
#include <stdint.h>
#include <string.h>

typedef struct fixedbugs_2346_b0 { int64_t a; unsigned b : 3; } fixedbugs_2346_b0;
typedef struct fixedbugs_2346_b1 { fixedbugs_2346_b0 left; fixedbugs_2346_b0 right; } fixedbugs_2346_b1;
typedef struct fixedbugs_2346_b2 { fixedbugs_2346_b1 left; fixedbugs_2346_b1 right; } fixedbugs_2346_b2;
typedef struct fixedbugs_2346_b3 { fixedbugs_2346_b2 left; fixedbugs_2346_b2 right; } fixedbugs_2346_b3;
typedef struct fixedbugs_2346_b4 { fixedbugs_2346_b3 left; fixedbugs_2346_b3 right; } fixedbugs_2346_b4;
typedef struct fixedbugs_2346_b5 { fixedbugs_2346_b4 left; fixedbugs_2346_b4 right; } fixedbugs_2346_b5;
typedef struct fixedbugs_2346_b6 { fixedbugs_2346_b5 left; fixedbugs_2346_b5 right; } fixedbugs_2346_b6;
typedef struct fixedbugs_2346_b7 { fixedbugs_2346_b6 left; fixedbugs_2346_b6 right; } fixedbugs_2346_b7;
typedef struct fixedbugs_2346_b8 { fixedbugs_2346_b7 left; fixedbugs_2346_b7 right; } fixedbugs_2346_b8;
typedef struct fixedbugs_2346_b9 { fixedbugs_2346_b8 left; fixedbugs_2346_b8 right; } fixedbugs_2346_b9;
typedef struct fixedbugs_2346_b10 { fixedbugs_2346_b9 left; fixedbugs_2346_b9 right; } fixedbugs_2346_b10;

static inline void fixedbugs_2346_b_set(fixedbugs_2346_b10 *v) {
    fixedbugs_2346_b0 *leaf = (fixedbugs_2346_b0 *)(void *)v;
    for (size_t i = 0; i < sizeof *v / sizeof *leaf; i++) {
        leaf[i].a = (int64_t)i;
        leaf[i].b = (unsigned)(i % 8);
    }
}

static inline fixedbugs_2346_b10 fixedbugs_2346_b_make(void) {
    fixedbugs_2346_b10 v;
    memset(&v, 0xFF, sizeof v);
    fixedbugs_2346_b_set(&v);
    return v;
}

static inline int64_t fixedbugs_2346_b_check(const fixedbugs_2346_b10 *p) {
    static fixedbugs_2346_b10 r;
    memset(&r, 0, sizeof r);
    fixedbugs_2346_b_set(&r);
    for (size_t i = 0; i < sizeof r; i++)
        if (((const unsigned char *)p)[i] != ((const unsigned char *)&r)[i]) return (int64_t)i + 1;
    return 0;
}

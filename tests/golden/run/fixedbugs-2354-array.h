/* Defect 2354: a group record of 128 MiB holding a fixed array of records,
 * each with a named bit-field.
 * C sets every byte between the first member and `rest` and the caller lends
 * the record again; C compares those bytes with `fixedbugs_2354_array_head`, the same members
 * before `rest` built from zero bytes with the members C set: 0 where every
 * byte agrees, else 1 plus the first that does not. The two structs share
 * their members up to `rest`, so C lays them out alike on every target, which
 * the assertion below holds. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct fixedbugs_2354_el { int8_t a; unsigned b : 3; } fixedbugs_2354_el;
typedef struct fixedbugs_2354_array { int64_t a; fixedbugs_2354_el e[4]; int64_t rest[1 << 24]; } fixedbugs_2354_array;
typedef struct fixedbugs_2354_array_head { int64_t a; fixedbugs_2354_el e[4]; int64_t rest0; } fixedbugs_2354_array_head;

_Static_assert(offsetof(fixedbugs_2354_array_head, rest0) == offsetof(fixedbugs_2354_array, rest), "the head lays its members out as the record does");

static inline void fixedbugs_2354_array_dirty(fixedbugs_2354_array *p) {
    memset((unsigned char *)p + sizeof p->a, 0xFF, offsetof(fixedbugs_2354_array, rest) - sizeof p->a);
}

static inline int64_t fixedbugs_2354_array_check(const fixedbugs_2354_array *p) {
    fixedbugs_2354_array_head r;
    memset(&r, 0, sizeof r);
    r.a = 7;
    for (int k = 0; k < 4; k++) {
        r.e[k].a = -1;
        r.e[k].b = 7;
    }
    for (size_t i = 0; i < offsetof(fixedbugs_2354_array, rest); i++)
        if (((const unsigned char *)p)[i] != ((const unsigned char *)&r)[i]) return (int64_t)i + 1;
    return 0;
}

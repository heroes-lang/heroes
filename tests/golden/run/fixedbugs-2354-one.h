/* Defect 2354: a group record of 128 MiB holding one named bit-field.
 * C sets every byte between the first member and `rest` and the caller lends
 * the record again; C compares those bytes with `fixedbugs_2354_one_head`, the same members
 * before `rest` built from zero bytes with the members C set: 0 where every
 * byte agrees, else 1 plus the first that does not. The two structs share
 * their members up to `rest`, so C lays them out alike on every target, which
 * the assertion below holds. */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct fixedbugs_2354_one { int64_t a; unsigned b : 3; int64_t rest[1 << 24]; } fixedbugs_2354_one;
typedef struct fixedbugs_2354_one_head { int64_t a; unsigned b : 3; int64_t rest0; } fixedbugs_2354_one_head;

_Static_assert(offsetof(fixedbugs_2354_one_head, rest0) == offsetof(fixedbugs_2354_one, rest), "the head lays its members out as the record does");

static inline void fixedbugs_2354_one_dirty(fixedbugs_2354_one *p) {
    memset((unsigned char *)p + sizeof p->a, 0xFF, offsetof(fixedbugs_2354_one, rest) - sizeof p->a);
}

static inline int64_t fixedbugs_2354_one_check(const fixedbugs_2354_one *p) {
    fixedbugs_2354_one_head r;
    memset(&r, 0, sizeof r);
    r.a = 7;
    r.b = 7;
    for (size_t i = 0; i < offsetof(fixedbugs_2354_one, rest); i++)
        if (((const unsigned char *)p)[i] != ((const unsigned char *)&r)[i]) return (int64_t)i + 1;
    return 0;
}

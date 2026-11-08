/* Defect 2338's binding: a record with padding bound `partial`, a C reader that
 * counts the padding bytes left non-zero, and a call that dirties the stack
 * first, so a byte the program never wrote reads as what the stack held. */
#include <stddef.h>
#include <stdint.h>

typedef struct fixedbugs_2338_padded {
    int8_t a;
    int64_t b;
    int8_t c;
    int64_t d;
} fixedbugs_2338_padded;

static int64_t fixedbugs_2338_dirty(const fixedbugs_2338_padded *p) {
    const unsigned char *q = (const unsigned char *)p;
    int64_t bad = 0;
    for (size_t i = offsetof(fixedbugs_2338_padded, a) + 1; i < offsetof(fixedbugs_2338_padded, b); i++) bad += q[i] != 0;
    for (size_t i = offsetof(fixedbugs_2338_padded, c) + 1; i < offsetof(fixedbugs_2338_padded, d); i++) bad += q[i] != 0;
    return bad;
}

static void fixedbugs_2338_scribble(void) {
    volatile unsigned char junk[65536];
    for (size_t i = 0; i < sizeof junk; i++) junk[i] = 0xAB;
}

#ifndef FFI_A_LENT_FIELD_READS_THROUGH_CONST_H
#define FFI_A_LENT_FIELD_READS_THROUGH_CONST_H

#include <stdint.h>

/* Panel 166, route H. A lend crosses as `const void *` from a name the
   program may not write and as `void *` from a `@` cell, and the header's
   own `const` is what decides, per call, whether C may write through it. */
typedef struct {
    unsigned char nsap[8];
    int64_t id;
} Slot;

static inline Slot slot_make(void) {
    Slot s = { {1, 0, 3, 0, 5, 0, 7, 0}, 7 };
    return s;
}

/* C READS: a `const` parameter takes a lend from any root. */
static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}

/* C WRITES: a parameter without `const` takes a lend from a `@` cell only. */
static inline void slot_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = (unsigned char)(i + 1);
}

#endif

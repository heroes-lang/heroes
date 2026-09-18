#ifndef FFI_A_LENT_FIELD_NEEDS_A_PLACE_H
#define FFI_A_LENT_FIELD_NEEDS_A_PLACE_H

#include <stdint.h>

typedef struct { unsigned char b[4]; int64_t id; } Slot;

static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *q = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += q[i];
    return s;
}

static inline Slot slot_make(void) {
    Slot s = {{1, 2, 3, 4}, 0};
    return s;
}

#endif

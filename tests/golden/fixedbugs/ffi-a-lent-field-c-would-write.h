#ifndef FFI_A_LENT_FIELD_C_WOULD_WRITE_H
#define FFI_A_LENT_FIELD_C_WOULD_WRITE_H

#include <stdint.h>

/* Defect 065's own shape: a byte field beside a sibling, filled by a C
   function whose parameter the header spells WITHOUT `const`. */
typedef struct {
    unsigned char nsap[8];
    int64_t id;
} Slot;

static inline Slot slot_make(void) {
    Slot s = { {72, 105, 0, 0, 0, 0, 0, 0}, 7 };
    return s;
}

static inline void slot_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = (unsigned char)(65 + i);
}

static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}

#endif

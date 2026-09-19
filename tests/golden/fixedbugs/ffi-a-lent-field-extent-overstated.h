#ifndef FFI_A_LENT_FIELD_EXTENT_OVERSTATED_H
#define FFI_A_LENT_FIELD_EXTENT_OVERSTATED_H

#include <stdint.h>

/* Defect 063's own shape: an 8-byte field with a sibling beside it, and a C
   function told the field is longer than it is. Both directions were silent at
   exit 0 for every overshoot from 9 to 4096 bytes. */
typedef struct {
    unsigned char name[8];
    int64_t id;
} Sl;

/* A struct Heroes declares `partial`: it names `name` and not the rest, so its
   size is C's and not the field list's. The check has to be right here too. */
typedef struct {
    unsigned char name[8];
    int64_t hidden_a;
    int64_t hidden_b;
} Both;

static inline Sl sl_make(void) {
    Sl s = { {1, 0, 3, 0, 5, 0, 7, 0}, 7 };
    return s;
}

static inline Both both_make(void) {
    Both b = { {1, 1, 1, 1, 1, 1, 1, 1}, 7, 7 };
    return b;
}

static inline void sl_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = 'A';
}

static inline int64_t sum_n(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}

#endif

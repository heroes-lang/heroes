#ifndef FFI_A_LENT_FIELD_COUNTED_BY_A_CONSTANT_H
#define FFI_A_LENT_FIELD_COUNTED_BY_A_CONSTANT_H

#include <stdint.h>

/* The extent as the header spells it: a macro, so a binding that names it is
   portable across platforms that size the field differently (panel 165's
   finding on L_tmpnam: 1024, 20 and 260 on the three). The emitted assertion
   writes the NAME and C compares its own value. */
#define SL_NAME_LEN 8

typedef struct {
    unsigned char name[SL_NAME_LEN];
    int64_t id;
} Sl;

/* Declared `partial` on the Heroes side: its size is C's. */
typedef struct {
    unsigned char name[SL_NAME_LEN];
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

static inline int64_t sum_n(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}

static inline void sl_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = (unsigned char)(i + 1);
}

#endif

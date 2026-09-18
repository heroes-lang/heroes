#ifndef FFI_A_BYTE_FIELD_CROSSES_TO_C_H
#define FFI_A_BYTE_FIELD_CROSSES_TO_C_H

#include <stdint.h>

/* The two shapes panel 164 was convened over, and neither reads to a
   terminator: the extent is stated by the CALL, which is what `const
   unsigned char *_LIBC_COUNT(__binlen)` means in the real `arpa/inet.h`. */
typedef struct {
    unsigned char nsap[8];
    signed char tag[4];
    int64_t id;
} Slot;

/* C READS the field. The length is the sibling argument, never a zero. */
static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}

/* C WRITES into the field. This is the direction that had no door at all:
   50 of 141 pointer parameters across 16 real headers are non-const. */
static inline void slot_fill(void *p, int64_t n) {
    unsigned char *b = (unsigned char *)p;
    for (int64_t i = 0; i < n; i++) b[i] = (unsigned char)(i + 1);
}

#endif

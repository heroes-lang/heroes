#ifndef FFI_COUNTED_BY_SHAPE_H
#define FFI_COUNTED_BY_SHAPE_H

#include <stdint.h>

typedef struct {
    unsigned char b[4];
    int64_t id;
} Slot;

static inline int64_t not_a_pointer(int64_t p, int64_t n) { return p + n; }
static inline int64_t no_such_sibling(const void *p, int64_t n) { (void)p; return n; }
static inline int64_t not_an_integer(const void *p, double n) { (void)p; return (int64_t)n; }
static inline int64_t out_parameter(const void *p, int64_t *n) { (void)p; return *n; }
static inline int64_t slot_sum(const void *p, int64_t n) {
    const unsigned char *b = (const unsigned char *)p;
    int64_t s = 0;
    for (int64_t i = 0; i < n; i++) s += b[i];
    return s;
}
static inline int64_t slot_count(const void *p, int64_t n) { return slot_sum(p, n); }

#endif

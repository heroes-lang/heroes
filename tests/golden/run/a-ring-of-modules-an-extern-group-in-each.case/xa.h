#ifndef XA_H
#define XA_H
#include <stdint.h>
typedef struct {
    int64_t w;
    int64_t h;
} XRect;
static inline int64_t xa_twice(int64_t n) { return n * 2; }
#endif

/* Defect 2389, 2026-11-09: a C function named `t1`, handed on as a value. */
#include <stdint.h>
static inline int64_t t1(int64_t x) { return x + 1; }

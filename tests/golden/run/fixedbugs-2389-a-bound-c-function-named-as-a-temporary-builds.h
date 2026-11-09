/* Defect 2389, 2026-11-09: two C functions named as the C the compiler writes
 * names its temporaries, `t1` and `t12`. */
#include <stdint.h>
static inline int64_t t1(int64_t x) { return x + 1; }
static inline int64_t t12(int64_t x) { return x + 12; }

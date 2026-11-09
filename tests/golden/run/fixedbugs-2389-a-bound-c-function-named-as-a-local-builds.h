/* Defect 2389, 2026-11-09: a C function named as the C the compiler writes
 * names a function's first local when it is `p`, `h0_p`. */
#include <stdint.h>
static inline int64_t h0_p(int64_t v) { return v + 2; }

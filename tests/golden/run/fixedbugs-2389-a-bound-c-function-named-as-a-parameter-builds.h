/* Defect 2389, 2026-11-09: two C functions named as the C the compiler writes
 * names a parameter `x`, `h0_x`, and the pointer an `@` parameter `n` arrives
 * through, `ph0_n`. */
#include <stdint.h>
static inline int64_t h0_x(int64_t v) { return v * 2; }
static inline int64_t ph0_n(int64_t v) { return v + 2; }

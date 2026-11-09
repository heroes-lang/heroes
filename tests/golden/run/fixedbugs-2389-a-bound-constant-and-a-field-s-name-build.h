/* Defect 2389, 2026-11-09: a constant named as a local `p` is in C, `h0_p`,
 * and a function named as a field `left` is, `f_left`. */
#include <stdint.h>
static const int64_t h0_p = 2;
static inline int64_t f_left(int64_t v) { return v + 1; }

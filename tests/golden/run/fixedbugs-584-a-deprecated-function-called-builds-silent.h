/* Defect 584, 2026-10-10: a deprecated function, bound and called. */
#include <stdint.h>
__attribute__((deprecated("use thrice"))) static inline int64_t twice(int64_t x) { return 2 * x; }

/* Defect 571, 2026-10-10: a deprecated function, bound and never called. */
#include <stdint.h>
__attribute__((deprecated("use thrice"))) static inline int64_t twice(int64_t x) { return 2 * x; }
static inline int64_t thrice(int64_t x) { return 3 * x; }

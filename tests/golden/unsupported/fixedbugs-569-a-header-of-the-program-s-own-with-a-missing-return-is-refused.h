/* Defect 569, 2026-10-10: a header whose own function misses a return. */
#include <stdint.h>
static inline int pick(int x) { if (x > 0) return 1; }

/* Defect 2389, 2026-11-09: the header of `fixedbugs-2389-a-header-s-macro-turning-a-temporary-into-another-is-told.hero`. */
#include <stdint.h>
#define t1 t2
static inline int64_t plus(int64_t x, int64_t y) { return x + y; }

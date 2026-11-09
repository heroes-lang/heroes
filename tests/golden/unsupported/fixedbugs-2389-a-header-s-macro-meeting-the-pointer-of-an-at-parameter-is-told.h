/* Defect 2389, 2026-11-09: the header of `fixedbugs-2389-a-header-s-macro-meeting-the-pointer-of-an-at-parameter-is-told.hero`. */
#include <stdint.h>
#define ph0_n 0
static inline int64_t plus(int64_t x, int64_t y) { return x + y; }

/* Defect 2389, 2026-11-09: the header of `fixedbugs-2389-a-header-s-macro-meeting-the-name-of-a-local-is-told.hero`. */
#include <stdint.h>
#define h0_p 0
static inline int64_t plus(int64_t x, int64_t y) { return x + y; }

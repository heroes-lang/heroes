/* Defect 2389, 2026-11-09: C functions named as the C the compiler writes
 * names a call's place, `hero_here`, and a map lookup's entry, `found`. */
#include <stdint.h>
static inline int64_t hero_here(int64_t x) { return x + 1; }
static inline int64_t found(int64_t x) { return x * 2; }

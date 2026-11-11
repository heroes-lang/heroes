/* Defect 2464, panel 245's R1, 2026-11-11: a door whose own code shifts an
 * int past its width, which C11 6.5.7p3 leaves undefined (the sitting
 * critic's shape `shift_count`). */
#include <stdint.h>
static inline int64_t door(int64_t x) { int y = 1 << 40; return x + y; }

/* Defect 2469, 2026-11-11: a header declaring a name with no type, which
 * C11 refuses; nothing of `stdio.h` is used. */
#include <stdint.h>
static implicit_x = 3;
static inline int64_t door(int64_t x) { return x + implicit_x; }

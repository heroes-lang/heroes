/* Defect 2044, 2026-10-13: a header that declares `int32_t` as another type
 * than the one C's <stdint.h> declares, `int` on every platform. */
typedef long int32_t;
static inline int seven(void) { return 7; }

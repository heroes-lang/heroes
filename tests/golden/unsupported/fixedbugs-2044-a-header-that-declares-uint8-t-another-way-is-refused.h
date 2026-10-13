/* Defect 2044, 2026-10-13: a header that declares `uint8_t` as another type
 * than the one C's <stdint.h> declares, `unsigned char` on every platform. */
typedef int uint8_t;
static inline int seven(void) { return 7; }

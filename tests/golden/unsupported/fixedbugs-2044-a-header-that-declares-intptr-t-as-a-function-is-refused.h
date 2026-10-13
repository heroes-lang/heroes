/* Defect 2044, 2026-10-13: a header that declares `intptr_t`, a name C's
 * <stdint.h> declares, as a function. */
static inline int intptr_t(void) { return 0; }
static inline int seven(void) { return 7; }

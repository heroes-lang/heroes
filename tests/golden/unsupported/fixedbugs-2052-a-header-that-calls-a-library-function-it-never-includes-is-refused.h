/* Defect 2052, 2026-10-13: a header that calls the C library's `abs`
 * without including `<stdlib.h>`, which declares it. */
static inline int eight(void) { return abs(-8); }

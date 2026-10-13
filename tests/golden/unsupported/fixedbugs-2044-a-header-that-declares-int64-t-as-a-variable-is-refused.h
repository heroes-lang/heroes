/* Defect 2044, 2026-10-13: a header that declares `int64_t`, a name C's
 * <stdint.h> declares, as a variable. */
int int64_t;
static inline int seven(void) { return 7; }

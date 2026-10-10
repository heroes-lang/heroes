/* Defect 569, 2026-10-10: a header with an unused variable in its own code. */
#include <stdint.h>
static int64_t unused_in_header(void) { int64_t x; return 0; }
static inline int64_t five(void) { return 5; }

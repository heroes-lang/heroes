/* Defect 568, 2026-10-10: a header that sets the switch first, then declares what it hides. */
#define _GNU_SOURCE 1
#include <stdint.h>
#ifdef _GNU_SOURCE
static inline int32_t hidden_cpu(void) { return 0; }
#endif

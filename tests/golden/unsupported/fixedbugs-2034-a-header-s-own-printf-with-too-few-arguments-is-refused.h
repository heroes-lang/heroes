/* Defect 2034, 2026-10-12: a header's own printf with more conversions than
 * arguments, which reads what the call never passed. */
#include <stdio.h>
static inline int fmt_shout(void) { return printf("%d %d\n", 7); }

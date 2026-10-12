/* Defect 2036, 2026-10-13: a header's own printf with one argument more than
 * its format names, which C11 7.21.6.1p2 evaluates and otherwise ignores. */
#include <stdio.h>
static inline int fmt_extra(void) { int n = printf("%d\n", 7, 8); fflush(stdout); return n; }

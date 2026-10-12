/* Defect 2036, 2026-10-13: a header's own printf whose format is a variable
 * and takes no argument, which C defines for the string it holds. */
#include <stdio.h>
static inline int fmt_plain(void) { const char *m = "hi\n"; int n = printf(m); fflush(stdout); return n; }

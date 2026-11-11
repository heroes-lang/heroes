/* Defect 2036, panel 245's R2, 2026-11-11: the known cost's fix, the
 * variable handed to a literal format as a string. */
#include <stdio.h>
static inline int fmt_plain(void) { const char *m = "hi\n"; int n = printf("%s", m); fflush(stdout); return n; }

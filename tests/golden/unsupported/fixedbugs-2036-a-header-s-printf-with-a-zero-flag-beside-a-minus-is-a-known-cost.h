/* Pinned 2026-10-13, a known cost by the author's ruling: a zero flag
 * beside a `-` in a header's own printf, which C defines and clang files
 * under -Wformat itself. */
#include <stdio.h>
static inline int fmt_shape(void) { return printf("%-05d\n", 5); }

/* Pinned 2026-10-13, a known cost by the author's ruling: a `bool` for
 * `%c` in a header's own printf, which C defines and clang files under
 * -Wformat itself. */
#include <stdbool.h>
#include <stdio.h>
static inline int fmt_shape(void) { return printf("%c\n", (bool)1); }

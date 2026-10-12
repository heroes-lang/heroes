/* Defect 2036, 2026-10-13: a header's own printf of an empty format handed
 * an argument, every argument an excess one (C11 7.21.6.1p2). */
#include <stdio.h>
static inline int fmt_empty(void) { int n = printf("", 5); fflush(stdout); return n; }

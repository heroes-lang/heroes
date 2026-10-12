/* Defect 2036, 2026-10-13: a header's own snprintf handed less room than its
 * output, which C11 7.21.6.5p2 cuts at n-1 characters, returning the whole
 * length: 5 and "hel", read back as 53. */
#include <stdio.h>
#include <string.h>
static inline int fmt_cut(void) { char b[4]; int n = snprintf(b, sizeof b, "hello"); return n * 10 + (int)strlen(b); }

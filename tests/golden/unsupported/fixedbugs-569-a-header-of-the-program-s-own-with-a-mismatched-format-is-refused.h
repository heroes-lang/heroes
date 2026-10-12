/* Defect 569, 2026-10-10, landed by defect 2034, 2026-10-12: a header whose
 * own function hands printf an argument its format does not match. */
#include <stdio.h>
static inline int fmt_shout(void) { return printf("%d\n", "seven"); }

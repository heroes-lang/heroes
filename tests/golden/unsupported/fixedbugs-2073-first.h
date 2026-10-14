/* Defect 2073, 2026-10-14: a header that needs <stdio.h> read before its
 * first line and includes it itself, below that line. */
#ifndef EOF
#error EOF is not defined yet
#endif
#include <stdio.h>
static inline int first_six(void) { return 6; }

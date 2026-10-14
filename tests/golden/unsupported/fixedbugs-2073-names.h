/* Defect 2073, 2026-10-14: a name declared only where <stdio.h> was read
 * before the header's first line, which includes it itself below. */
#ifdef EOF
static inline int names_val(void) { return 3; }
#endif
#include <stdio.h>
static inline int names_other(void) { return 1; }

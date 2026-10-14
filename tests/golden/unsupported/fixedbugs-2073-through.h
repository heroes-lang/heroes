/* Defect 2073, 2026-10-14: the same, reaching <stdio.h> through a header of
 * its own. */
#ifndef EOF
#error EOF is not defined yet
#endif
#include "fixedbugs-2073-mid.h"
static inline int through_six(void) { return 6; }

/* Defect 158, lane h158, 2026-10-02: a group's header that includes a
   second header, which includes one no machine has. */
#include <stdint.h>
#include "fixedbugs-158-depth-two-mid.h"

static inline int32_t seven(void) { return 7; }

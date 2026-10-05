/* Defect 360, lane cli12, 2026-10-05: a group's header that compiles, and
   includes one that does not. */
#include <stdint.h>
#include "fixedbugs-360-inner.h"
static inline int64_t four(void) { return 4; }

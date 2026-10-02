/* Defect 158, lane h158, 2026-10-02: a group's header that includes another
   group's header. */
#include <stdint.h>
#include "fixedbugs-158-inner-group.h"

static inline int32_t seven(void) { return 7; }

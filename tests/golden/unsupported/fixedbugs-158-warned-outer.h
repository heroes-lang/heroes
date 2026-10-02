/* Defect 158, lane h158, 2026-10-02: a warning here and one in the header
   this one includes, so clang prints a stack above each warning and none
   above the error. */
#include <stdint.h>
#warning "a warning in the group's header"
#include "fixedbugs-158-warned-mid.h"

static inline int32_t seven(void) { return 7; }

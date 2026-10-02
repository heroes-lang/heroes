/* Defect 158, lane h158, 2026-10-02: the same include in quotes, which
   clang looks for beside this file first and then on the include path. */
#include <stdint.h>
#include "no_such_header_here.h"

static inline int32_t seven(void) { return 7; }

/* Defect 158, lane h158, 2026-10-02: a macro defined here and again in the
   header this one includes, so clang's warning there carries a note
   pointing back here. */
#include <stdint.h>
#define FIXEDBUGS_158_TWICE 1
#include "fixedbugs-158-noted-mid.h"

static inline int32_t seven(void) { return 7; }

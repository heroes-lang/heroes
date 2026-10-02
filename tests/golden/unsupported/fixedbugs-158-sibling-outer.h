/* Defect 158, lane h158, 2026-10-02: two headers side by side, a warning
   in the first and the missing include in the second. */
#include <stdint.h>
#include "fixedbugs-158-sibling-a.h"
#include "fixedbugs-158-sibling-b.h"

static inline int32_t seven(void) { return 7; }

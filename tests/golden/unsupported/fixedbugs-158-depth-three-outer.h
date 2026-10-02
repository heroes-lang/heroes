/* Defect 158, lane h158, 2026-10-02: three headers deep, in quotes, then
   angle brackets, then quotes. */
#include <stdint.h>
#include "fixedbugs-158-depth-three-mid.h"

static inline int32_t seven(void) { return 7; }

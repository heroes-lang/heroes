/* Defect 158, lane h158, 2026-10-02: a name marked deprecated here and used in the header this one includes, so clang's warning there carries a note pointing back here (a macro defined again was the first carrier, an error since panel 204's R2).
   The header's own pragma keeps that warning one, since a region that quiets a deprecation would leave the note nothing to point from (panel 245's route (d); its compiler-engineer's line). */
#pragma clang diagnostic warning "-Wdeprecated-declarations"
#include <stdint.h>
extern int fixedbugs_158_old __attribute__((deprecated));
#include "fixedbugs-158-noted-mid.h"

static inline int32_t seven(void) { return 7; }

/* Defect 158, lane h158, 2026-10-02: a macro defined here and again in the
   header this one includes, so clang's warning there carries a note
   pointing back here; since 2026-10-10 a name marked deprecated here and used there, a macro defined again being an error (panel 204's R2). */
#include <stdint.h>
extern int fixedbugs_158_old __attribute__((deprecated));
#include "fixedbugs-158-noted-mid.h"

static inline int32_t seven(void) { return 7; }

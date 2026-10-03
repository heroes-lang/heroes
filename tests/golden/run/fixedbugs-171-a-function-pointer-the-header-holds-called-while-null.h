/* Beside defect 171's run case, lane ffimsg, 2026-10-03: a name the header
   holds as a POINTER to a function, null until something sets it. */
#include <stdint.h>

static int32_t (*hook)(int32_t) = 0;

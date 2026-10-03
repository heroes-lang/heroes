/* Beside defect 171's run cases, lane ffimsg, 2026-10-03: a macro over
   `(*slot)`, which C reads as a function at every question a compile can
   ask, and which calls through a pointer at run time. */
#include <stdint.h>

static int32_t (*slot)(int32_t) = 0;
#define via_slot (*slot)

/* Defect 582, 2026-10-10: a push of the header's own, never popped. */
#include <stdlib.h>
#pragma float_control(push)
#pragma float_control(precise, off)

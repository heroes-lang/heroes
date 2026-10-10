/* Defect 582, 2026-10-10: a pop past the header's own pushes. */
#include <stdlib.h>
#pragma float_control(pop)
#pragma clang fp contract(fast)

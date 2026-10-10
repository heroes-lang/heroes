/* Defect 582, 2026-10-10: a header that pops a label it never pushed. */
#pragma pack(push, outer, 1)
#include <stdlib.h>
#pragma pack(pop, inner)

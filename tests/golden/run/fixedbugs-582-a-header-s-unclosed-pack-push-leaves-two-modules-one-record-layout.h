/* Defect 582, 2026-10-10: a header that packs and never pops. */
#pragma pack(push, 1)
#include <stdlib.h>

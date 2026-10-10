/* Defect 582, 2026-10-10: contraction across statements, never reset. */
#include <stdlib.h>
#pragma clang fp contract(fast)

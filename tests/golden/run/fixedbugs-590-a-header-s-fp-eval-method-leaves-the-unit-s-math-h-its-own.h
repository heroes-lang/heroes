/* Defect 590, 2026-10-11: an evaluation method set at file scope, never
   reset, which made clang refuse the unit's own <math.h>. */
#pragma clang fp eval_method(double)
#include <stdlib.h>

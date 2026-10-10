/* Defect 592, 2026-10-11: a header renaming `main`, which the unit
   defines after its groups and the C library's start calls. */
#pragma redefine_extname main fixedbugs_592_other_main
#include <stdlib.h>

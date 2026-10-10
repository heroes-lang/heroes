/* Defect 592, 2026-10-11: a header renaming a function of the compiler's
   runtime, which every unit declares before its groups. */
#pragma redefine_extname hero_print_int fixedbugs_592_print_int
#include <stdlib.h>

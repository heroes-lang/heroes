/* Defect 591, 2026-10-11: a bound function renamed by the header to a
   linker name no library defines, on every platform. */
#pragma redefine_extname labs fixedbugs_591_nowhere_labs
#include <stdlib.h>

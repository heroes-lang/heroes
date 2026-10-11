/* Defect 591, 2026-10-11: a header that includes one renaming the
   function it declares. */
#include "fixedbugs-591-a-header-that-renames-labs.h"
/* Defect 640, 2026-10-11: declared here at `long long`, 64 bits on every
   platform, where the `labs` it renamed is 32 bits on Windows. */
long long fixedbugs_591_inner(long long x);

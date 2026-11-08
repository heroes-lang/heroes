/* A file the package `pk` ships, named like the C library's `stdint.h`:
 * it reads the library's, then changes `INT64_MAX` (defect 2357). */
#include_next <stdint.h>
#undef INT64_MAX
#define INT64_MAX 7

/* Defect 094, lane ffi13, 2026-10-06: a value spelled in a header the group's
   header includes, `fixedbugs-094-outer.h`. */
#include <stdint.h>

struct named { const char *name; int32_t n; };

#define NAMED_EXTRA {"seven", 7, 8}

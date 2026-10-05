/* Defect 360, lane cli12, 2026-10-05: a header that stops itself with an
   `#error` directive, as a library does on a configuration it refuses. */
#include <stdint.h>
#error "this library is configured for another platform"
static inline int64_t five(void) { return 5; }

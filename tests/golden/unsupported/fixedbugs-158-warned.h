/* Defect 158, lane h158, 2026-10-02: a warning above the include, so clang
   prints its include stack above the warning and none above the error. */
#include <stdint.h>
#warning "a warning clang prints before the include it cannot open"
#include <no_such_header_here.h>

static inline int32_t seven(void) { return 7; }

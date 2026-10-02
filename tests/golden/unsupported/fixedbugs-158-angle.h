/* Defect 158, lane h158, 2026-10-02: a header of the program's own that
   includes, in angle brackets, a header no machine has. */
#include <stdint.h>
#include <no_such_header_here.h>

static inline int32_t seven(void) { return 7; }

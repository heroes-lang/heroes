/* Defect 158, lane h158, 2026-10-02: the second group's header, which
   includes a header no machine has. */
#include <stdint.h>
#include <no_such_header_here.h>

static inline int32_t eight(void) { return 8; }

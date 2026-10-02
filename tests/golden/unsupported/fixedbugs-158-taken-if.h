/* Defect 158, lane h158, 2026-10-02: a missing include under an `#if`
   clang does not take, and one under an `#if` it does. */
#include <stdint.h>
#if 0
#include <never_read_here.h>
#endif
#if 1
#include <no_such_header_here.h>
#endif

static inline int32_t seven(void) { return 7; }

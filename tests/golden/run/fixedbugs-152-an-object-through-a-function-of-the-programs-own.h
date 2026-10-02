/* Beside defect 152's run case, lane ffi-macro, 2026-10-02: the function of
   the program's own that `ffi_call_shape` names for a C object, here
   `errno`, which no group member binds. Its type is the header's own, so the
   probe holds the declaration against it like any function's. */
#include <errno.h>

static inline void hero_errno_clear(void) { errno = 0; }
static inline int hero_errno(void) { return errno; }

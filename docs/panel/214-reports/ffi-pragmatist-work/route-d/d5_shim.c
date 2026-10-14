#include <stdint.h>
#include "d5_wrap.h"
int32_t hero_shim_exit_status(int32_t status) { return exit_status(status); }
int64_t hero_shim_SQLITE_ROW(void) { return SQLITE_ROW; }
_Static_assert(__builtin_constant_p(SQLITE_ROW), "heroes-ffi-const SQLITE_ROW");

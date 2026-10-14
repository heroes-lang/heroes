/* Today: the header in the program's own unit. */
#include <stdint.h>
#include "d5_wrap.h"
int64_t h_main_SQLITE_ROW(void) { return SQLITE_ROW; }
int64_t h_main_f(int32_t s) { return (int64_t)exit_status(s) + h_main_SQLITE_ROW(); }

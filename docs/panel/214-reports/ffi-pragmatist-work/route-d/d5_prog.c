/* Route D: the program's unit never reads the header. */
#include <stdint.h>
int32_t hero_shim_exit_status(int32_t status);
int64_t hero_shim_SQLITE_ROW(void);
int64_t h_main_f(int32_t s) { return (int64_t)hero_shim_exit_status(s) + hero_shim_SQLITE_ROW(); }

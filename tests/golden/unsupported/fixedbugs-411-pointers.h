/* Beside defect 411's cases: functions the header hands out through function
   pointer VARIABLES, which a binding names as it names a function. One writes
   the bytes it is handed; the other writes a `size_t` through its argument. */
#include <stdint.h>
#include <stddef.h>

static int32_t hero_write_first(unsigned char *p) { p[0] = 'Z'; return 1; }
static int32_t (*fp_write)(unsigned char *) = hero_write_first;
static void hero_fill_count(size_t *n) { *n = (size_t)-1; }
static void (*fp_count)(size_t *) = hero_fill_count;

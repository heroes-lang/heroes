/* A C function that hands back bytes through a `const char **` cell: the
 * const says the caller is asked not to change them, which is how C lends a
 * string, as `sqlite3_prepare_v2`'s `pzTail` does. The program beside this
 * marks the cell `owned`, which says the opposite. */
#include <stdlib.h>
#include <stdint.h>
static inline __attribute__((noinline)) void fill_out(const char **out) { *out = "lent"; }
static inline __attribute__((noinline)) void free_out(const char *p) { free((void *)(uintptr_t)p); }

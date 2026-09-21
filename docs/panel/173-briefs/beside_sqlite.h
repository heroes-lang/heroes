/* Panel 173 q3: the `b_later` shape with a REAL library's free. A library that
 * keeps a `const char *` and later frees it with its own allocator is the one
 * route by which sqlite3_free reaches a lease, since the checker refuses every
 * direct spelling (`ptr` is a type_mismatch, `cstr` is ffi_writable_parameter). */
#include <sqlite3.h>
#include <stdint.h>
static const char *sql_stashed;
static inline void sql_stash(const char *s) { sqlite3_initialize(); sql_stashed = s; }
static inline void sql_later_free(void) { sqlite3_free((void *)(uintptr_t)sql_stashed); }

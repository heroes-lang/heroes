/* A C side that KEEPS what it is handed: defect 024's own shape, the one no
 * position rule can see, because SQLITE_STATIC and SQLITE_TRANSIENT are both
 * `const char *` in the header. */
#include <string.h>
static const char *held_by_c;
static void stash_put(const char *s) { held_by_c = s; }
static unsigned long stash_len(void) { return strlen(held_by_c); }

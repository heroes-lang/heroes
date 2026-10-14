/* Route F on Darwin: the emitter's `h_` moved into C's reserved space as
 * `__h_`; a module `errno` with a function `set` (today `h_errno_set`), in
 * a program with a group naming netdb.h. */
#include <stdint.h>
#include <netdb.h>
int64_t __h_errno_set(int64_t v) { return v; }

/* Route F on glibc: a module `errno` with a function `location` (today
 * `h_errno_location`, measured with --emit-c), with netdb.h. */
#include <stdint.h>
#include <netdb.h>
int64_t __h_errno_location(void) { return 7; }

/* Panel 173 q3: does `ffi_writable_parameter` also shut a `char *` taker? A
 * library that takes ownership of a string usually declares `char *`. */
#include <stdlib.h>
static inline void take_owner(char *s) { free(s); }

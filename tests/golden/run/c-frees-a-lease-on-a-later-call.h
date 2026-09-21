/* Panel 172's `b_later`: C keeps the lease on one call and frees it on a LATER
 * call that takes no pointer at all. There is no parameter anywhere to write a
 * word on; this is the shape that decided the sitting against a vocabulary. */
#include <stdlib.h>
#include <stdint.h>
static const char *stashed;
static inline void stash(const char *s) { stashed = s; }
static inline void later_free(void) { free((void *)(uintptr_t)stashed); }

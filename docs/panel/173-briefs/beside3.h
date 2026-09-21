#include <assert.h>
static inline void look3(const char *s) { (void)s; }
/* A C library's own assertion, failing for C's own reason. */
static inline void cassert(void) { assert(1 == 2); }

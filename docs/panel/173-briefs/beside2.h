#include <stdlib.h>
#include <assert.h>
static inline void look2(const char *s) { (void)s; }
/* C's own abort, for C's own reason: nothing was freed. */
static inline void boom(void) { abort(); }
/* A write through a null pointer while a lease is live. */
static inline void poke(void) { *(volatile int *)0 = 1; }

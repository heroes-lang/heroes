/* A C library that hands out a handle it keeps (`borrows`), whose close really
 * frees. Given back twice, the second close is a double free inside C.
 * noinline so each call stays a call. */
#include <stdlib.h>
typedef struct gg gg;
static inline __attribute__((noinline)) gg *g_open(void) { return (gg *)malloc(8); }
static inline __attribute__((noinline)) void g_close(gg *x) { free(x); }

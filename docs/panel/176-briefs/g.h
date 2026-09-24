/* Panel 175 critic: a getter that hands back the handle it was given. */
#include <stdlib.h>
typedef struct gg gg;
static __attribute__((noinline)) gg *g_open(void) { return (gg *)malloc(8); }
static __attribute__((noinline)) gg *g_same(gg *x) { return x; }
static __attribute__((noinline)) void g_close(gg *x) { free(x); }

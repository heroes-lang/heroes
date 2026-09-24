/* `freopen`'s shape: a call that consumes a handle and acquires the next one
 * in the same family. It may take back what its own releaser is owed — an
 * `h_open` handle — and never a `p_open` one, which is owed `p_close`. */
#include <stdint.h>
typedef struct hh { int64_t n; } hh;
static inline __attribute__((noinline)) hh *h_open(int64_t n) { static hh pool[4]; static int64_t next = 0; pool[next].n = n; return &pool[next++]; }
static inline __attribute__((noinline)) hh *h_reopen(hh *x, int64_t n) { x->n = n; return x; }
static inline __attribute__((noinline)) void h_close(hh *x) { (void)x; }
static inline __attribute__((noinline)) int64_t h_value(hh *x) { return x->n; }

/* A handle with two releasers, the way SQLite's connection has two: either one
 * ends the life, and a program may pick. A pool, so the case counts without
 * an allocator and runs on every leg. */
#include <stdint.h>
typedef struct hh { int64_t n; } hh;
static inline __attribute__((noinline)) hh *h_open(int64_t n) { static hh pool[4]; static int64_t next = 0; pool[next].n = n; return &pool[next++]; }
static inline __attribute__((noinline)) void h_close(hh *x) { (void)x; }
static inline __attribute__((noinline)) void h_close_v2(hh *x) { (void)x; }
static inline __attribute__((noinline)) int64_t h_value(hh *x) { return x->n; }

/* A group record holding a fixed array of records and a fixed array of
 * integers, and a C out-parameter: `put_seven` writes through the pointer it is
 * handed, which is the copy-back temporary when the argument is an element.
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdint.h>

typedef struct cell {
    int64_t v;
    int64_t w;
} cell;

typedef struct holder {
    cell xs[3];
    int64_t a[4];
} holder;

static inline __attribute__((noinline)) void put_seven(int64_t *out) { *out = 7; }

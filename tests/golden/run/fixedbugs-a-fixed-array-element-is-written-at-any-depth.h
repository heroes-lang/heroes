/* Every nesting a group record allows around a fixed array (spec § 13: a field
 * is a number, another record of the group, or a fixed array of one): an array
 * of records that hold arrays, an array of records written by field, and C
 * functions that read what Heroes wrote back, so a write into a copy shows.
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdint.h>

typedef struct row {
    int64_t a[3];
} row;

typedef struct outer {
    row rows[2];
    int32_t tail;
} outer;

typedef struct cell {
    int64_t v;
    int64_t w;
} cell;

typedef struct holder {
    cell xs[3];
} holder;

typedef struct nums {
    int64_t a[4];
} nums;

static inline __attribute__((noinline)) int64_t outer_sum(outer o) {
    int64_t total = o.tail;
    for (int r = 0; r < 2; r++) {
        for (int i = 0; i < 3; i++) {
            total += o.rows[r].a[i];
        }
    }
    return total;
}

static inline __attribute__((noinline)) int64_t holder_sum(holder h) {
    return h.xs[0].v + h.xs[0].w + h.xs[1].v + h.xs[1].w + h.xs[2].v + h.xs[2].w;
}

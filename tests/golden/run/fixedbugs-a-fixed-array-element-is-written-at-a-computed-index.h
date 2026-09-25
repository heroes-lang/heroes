/* Four narrow integers in a group record, read back by C. The element is an
 * `int32_t` on purpose: a write that lowered the subscript to the wrong width
 * would move the neighbours, and C's sum would say so.
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdint.h>

typedef struct quad {
    int32_t a[4];
    int64_t after;
} quad;

static inline __attribute__((noinline)) int64_t quad_sum(quad q) {
    return (int64_t)q.a[0] + q.a[1] + q.a[2] + q.a[3] + q.after;
}

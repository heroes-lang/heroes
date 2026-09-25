/* A group record whose only field is a fixed array, and the sum that reads it
 * back on the C side: the case asks C, not Heroes, whether the write landed,
 * because a write into a copy would leave C reading the old element.
 *
 * Shipped beside the case so it fires on all three platforms. `static inline`
 * because this header reaches a second translation unit that calls nothing, and
 * `noinline` so the struct crosses by value as a real call. */
#include <stdint.h>

typedef struct nums {
    int64_t a[4];
} nums;

static inline __attribute__((noinline)) int64_t nums_sum(nums n) {
    return n.a[0] + n.a[1] + n.a[2] + n.a[3];
}

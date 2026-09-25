/* A fixed array with a field BEHIND it, so an unguarded write one past the
 * last element would land in `after` rather than trap: AddressSanitizer cannot
 * see an overflow that stays inside one C struct, which is why the bound is the
 * compiler's (panel 062).
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdint.h>

typedef struct nums {
    int64_t a[4];
    int64_t after;
} nums;

static inline __attribute__((noinline)) int64_t nums_after(nums n) { return n.after; }

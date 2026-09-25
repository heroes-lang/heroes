/* One fixed array of every element kind a group field may hold besides a
 * record and a handle, which have cases of their own: a byte, a `bool`, a
 * narrow float, a `ptr`, a `cstr` and a narrow signed integer. `kinds_check`
 * reads each written element back on the C side, weighted so every element
 * shows in the one number it returns.
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdbool.h>
#include <stdint.h>

typedef struct kinds {
    uint8_t name[8];
    bool flags[2];
    float xs[2];
    void *ps[2];
    const char *labels[2];
    int16_t small[3];
} kinds;

static inline __attribute__((noinline)) const char *kinds_label(void) { return "label"; }

static inline __attribute__((noinline)) void *kinds_pointer(void) {
    static int64_t target = 0;
    return &target;
}

static inline __attribute__((noinline)) int64_t kinds_check(kinds k) {
    int64_t total = k.name[0];
    total += k.flags[1] ? 1000 : 0;
    total += (int64_t)(k.xs[1] * 10.0f);
    total += k.ps[0] == 0 ? 0 : 20000;
    total += k.labels[1] == 0 ? 0 : 300000;
    total += k.small[2];
    return total;
}

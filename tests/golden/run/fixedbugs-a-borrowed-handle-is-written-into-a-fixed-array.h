/* A handle C keeps for itself, handed out by `ob_new` from a static pool, and
 * a group record holding four of them inline. No allocator and no release: the
 * handle is borrowed, so the program owes C nothing for it.
 *
 * Shipped beside the case so it fires on all three platforms; `static inline`
 * and `noinline` as every header in this directory. */
#include <stdint.h>

typedef struct ob {
    int64_t n;
} ob;

typedef struct four {
    ob *a[4];
} four;

static inline __attribute__((noinline)) ob *ob_new(int64_t n) {
    static ob pool[8];
    static int64_t next = 0;
    pool[next].n = n;
    return &pool[next++];
}

static inline __attribute__((noinline)) int64_t four_sum(four f) {
    int64_t total = 0;
    for (int i = 0; i < 4; i++) {
        total += f.a[i] == 0 ? 0 : f.a[i]->n;
    }
    return total;
}

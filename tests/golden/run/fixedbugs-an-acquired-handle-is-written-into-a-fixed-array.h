/* A handle whose life the program owns: `ob_new` begins it from a static pool
 * and `ob_free` ends it, so four of them can be counted without an allocator.
 * `four_sum` reads the record back, so C sees what Heroes wrote.
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

static int64_t ob_freed_count = 0;
static inline __attribute__((noinline)) void ob_free(ob *o) { o->n = 0; ob_freed_count++; }
static inline __attribute__((noinline)) int64_t ob_freed(void) { return ob_freed_count; }

static inline __attribute__((noinline)) int64_t four_sum(four f) {
    int64_t total = 0;
    for (int i = 0; i < 4; i++) {
        total += f.a[i] == 0 ? 0 : f.a[i]->n;
    }
    return total;
}

/* The object of `dead-handle-read-by-c-through-a-cell-it-should-only-write.h`
 * with a field at offset 72, read through the cell (defect 115, 2026-09-27). */
#include <stdint.h>

typedef struct ob ob;

struct ob {
    int64_t refs;
    int64_t alive;
    char pad[56];
    int64_t far;
};

static inline __attribute__((noinline)) ob *ob_new(void) {
    static ob pool[4];
    static int64_t next = 0;
    ob *o = &pool[next++];
    o->refs = 1;
    o->alive = 1;
    return o;
}

static inline __attribute__((noinline)) void ob_put(ob *o) {
    o->refs--;
    if (o->refs == 0) o->alive = 0;
}

static inline __attribute__((noinline)) int64_t ob_refs(ob *o) { return o->refs; }

static inline __attribute__((noinline)) int32_t ob_far_cell(ob **o) { return (int32_t)((*o)->far + 1); }

static inline __attribute__((noinline)) int64_t one(void) { return 1; }

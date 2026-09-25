/* A reference-counted object over a static pool, and a C function that
 * READS the handle in its cell and adds a reference (the landing's skeptic,
 * probe `a2d`, 2026-09-25): the shape a binding must spell `retains` WITHOUT
 * `@`, and this one is bound with it. */
#include <stdint.h>

typedef struct ob ob;

struct ob {
    int64_t refs;
    int64_t alive;
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

static inline __attribute__((noinline)) int32_t ob_up_ref_cell(ob **o) {
    (*o)->refs++;
    return 1;
}

static inline __attribute__((noinline)) int64_t one(void) { return 1; }

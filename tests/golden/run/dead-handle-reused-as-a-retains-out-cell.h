/* A reference-counted object, X509 in miniature, over a static pool, and a
 * getter that hands a reference OUT into a cell it only writes (the
 * landing's skeptic, probe `a2e`, 2026-09-25). */
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

static ob *ob_shared = 0;

static inline __attribute__((noinline)) int32_t ob_get_shared(ob **out) {
    if (ob_shared == 0) ob_shared = ob_new();
    ob_shared->refs++;
    *out = ob_shared;
    return 1;
}

static inline __attribute__((noinline)) int64_t one(void) { return 1; }

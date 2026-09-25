#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
static ob one;
static inline __attribute__((noinline)) ob *ob_new(void) { one.refs = 1; return &one; }
/* The count read from the pool and not through a handle: after `both_put` the
 * program's handles are dead (panel 177), and this is how the case sees C's side. */
static inline __attribute__((noinline)) int64_t ob_last_refs(void) { return one.refs; }
static inline __attribute__((noinline)) ob *ob_get(ob *o) { o->refs++; return o; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) void both_put(ob *a, ob *b) { a->refs--; b->refs--; }
static inline __attribute__((noinline)) int64_t ob_refs(ob *o) { return o->refs; }

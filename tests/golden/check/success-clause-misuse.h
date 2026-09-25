#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) int32_t ob_peek(ob *o) { return (int32_t)o->refs; }
static inline __attribute__((noinline)) void ob_take(ob *o) { o->refs--; }
static inline __attribute__((noinline)) int32_t ob_give(ob *o) { o->refs--; return 0; }
static inline __attribute__((noinline)) int ob_test(ob *o) { o->refs--; return 1; }
static inline __attribute__((noinline)) void ob_both(ob *o) { o->refs--; }
static inline __attribute__((noinline)) ob *ob_get(ob *o) { o->refs++; return o; }

#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) ob *ob_get(ob *o) { o->refs++; return o; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) void ob_drop(ob *o) { o->refs--; }

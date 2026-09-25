#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
typedef struct wr { ob *inner; } wr;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) wr *wr_new(ob *o) { static wr one; one.inner = o; return &one; }
static inline __attribute__((noinline)) void wr_free(wr *w) { ob_put(w->inner); w->inner = 0; }

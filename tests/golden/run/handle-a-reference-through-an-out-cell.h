/* A reference handed OUT through a cell with a status, `ob_dup(o, &out)`:
 * the cell receives the same object, one reference more. */
#include <stdint.h>
typedef struct ob { int64_t refs; } ob;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) int32_t ob_dup(ob *o, ob **out) { o->refs++; *out = o; return 1; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) int64_t ob_refs(ob *o) { return o->refs; }

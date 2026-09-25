#include <stdint.h>
#include <stdbool.h>
typedef struct ob { int64_t refs; } ob;
static inline __attribute__((noinline)) ob *ob_new(void) { static ob pool[2]; static int next = 0; pool[next].refs = 1; return &pool[next++]; }
static inline __attribute__((noinline)) uint64_t ob_release_all(ob *o) { o->refs--; return UINT64_MAX; }
static inline __attribute__((noinline)) bool ob_try_put(ob *o) { o->refs--; return true; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }

/* Two ways C adds a reference: returned, the way json-c's `json_object_get`
 * returns the object it was given, and through a parameter with a status,
 * the way OpenSSL's `X509_up_ref` returns 1 for success. A count in the pool
 * stands for the library's own refcount, and the release ends the life only
 * at zero, so the program's releases are what C expects. */
#include <stdint.h>
typedef struct ob { int64_t refs; int64_t v; } ob;
static inline __attribute__((noinline)) ob *ob_new(int64_t v) { static ob pool[4]; static int next = 0; pool[next].refs = 1; pool[next].v = v; return &pool[next++]; }
static inline __attribute__((noinline)) ob *ob_get(ob *o) { o->refs++; return o; }
static inline __attribute__((noinline)) int32_t ob_up_ref(ob *o) { o->refs++; return 1; }
static inline __attribute__((noinline)) void ob_put(ob *o) { o->refs--; }
static inline __attribute__((noinline)) int64_t ob_refs(ob *o) { return o->refs; }

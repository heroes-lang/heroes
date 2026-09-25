/* OpenSSL's X509_new / X509_up_ref / X509_free in miniature, for the
 * llm-ergonomist's A3 at panel 177: a reference-counted object over a static
 * pool, whose `up_ref` answers 1 on success as X509_up_ref does. */
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

static inline __attribute__((noinline)) int32_t ob_up_ref(ob *o) {
    o->refs++;
    return 1;
}

static inline __attribute__((noinline)) void ob_put(ob *o) {
    o->refs--;
    if (o->refs == 0) o->alive = 0;
}

static inline __attribute__((noinline)) int64_t ob_refs(ob *o) { return o->refs; }

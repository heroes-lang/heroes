#include <stdint.h>
#include <stdlib.h>

/* A counter C makes and frees: a C handle, its life the runtime's to follow
 * from the thread that holds it to the thread a mailbox hands it to. */
typedef struct counter {
    int64_t n;
} counter;

static inline counter *counter_new(int64_t n) {
    counter *c = (counter *)malloc(sizeof *c);
    if (c != 0) c->n = n;
    return c;
}

static inline void counter_free(counter *c) { free(c); }

static inline int64_t counter_add(counter *c, int64_t by) {
    c->n += by;
    return c->n;
}

/* A struct the header hands back by pointer and a function that READS through
 * it, which is what every `getaddrinfo`-shaped binding does. `node_value`
 * dereferences without checking, and that is not a bug in this header: a C
 * function documenting a non-null parameter is entitled to assume one, and the
 * Heroes program is the one holding `nullptr`. */
#include <stdint.h>

struct node {
    int64_t value;
    struct node *next;
};

static inline struct node *node_open(int64_t v) {
    static struct node pool[1];
    pool[0].value = v;
    pool[0].next = 0;
    return &pool[0];
}

static inline int64_t node_value(struct node *p) { return p->value; }

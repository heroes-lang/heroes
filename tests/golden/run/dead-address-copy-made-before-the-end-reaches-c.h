/* Nodes from a static pool that is never reused: a freed node keeps its
 * address and reads -1, so a stale copy that reached C would print -1 at
 * exit 0, which is the silent wrong answer this case exists to refuse. */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v;
};

static inline __attribute__((noinline)) node *node_new(void) {
    static node pool[8];
    static int64_t next = 0;
    node *n = &pool[next++];
    n->v = next;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) { n->v = -1; }

static inline __attribute__((noinline)) int64_t node_value(node *n) { return n->v; }

/* Same-size nodes from a pool with a FREE LIST, the shape the llm-ergonomist
 * named at panel 176 (`reuse_malloc`): a node given back, a new one made at
 * the same address, the old one given back again. The free list is what
 * makes the reuse deterministic; libmalloc's is not (defect 077: two
 * byte-identical binaries read 0 and 134 by their code signature alone). */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v;
    node *next_free;
};

static node node_pool[8];
static node *node_free_list = 0;
static int64_t node_next_fresh = 0;
static int64_t node_made = 0;

static inline __attribute__((noinline)) node *node_new(void) {
    node *n;
    if (node_free_list != 0) {
        n = node_free_list;
        node_free_list = n->next_free;
    } else {
        n = &node_pool[node_next_fresh++];
    }
    node_made++;
    n->v = node_made;
    n->next_free = 0;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) {
    n->v = -1;
    n->next_free = node_free_list;
    node_free_list = n;
}

static inline __attribute__((noinline)) int64_t node_value(node *n) { return n->v; }

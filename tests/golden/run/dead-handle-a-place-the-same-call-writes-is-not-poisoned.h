/* Same-size nodes from a pool with a FREE LIST, so the node `node_swap`
 * gives back is the one it hands out again, at the same address, every time
 * (the landing's skeptic, probe `a14`, 2026-09-25). */
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

/* Ends the node it is handed and writes a new one into the cell: the pool
 * hands the freed address straight back. */
static inline __attribute__((noinline)) void node_swap(node *n, node **out) {
    node_free(n);
    *out = node_new();
}

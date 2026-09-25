/* A C library that keeps a callback AND a node the program handed it, and
 * calls back later with the node (the landing's skeptic, probe `e4`,
 * 2026-09-25). A static pool, so a read of the freed node is silent and
 * reads -1. */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v;
};

static inline __attribute__((noinline)) node *node_new(void) {
    static node pool[4];
    static int64_t next = 0;
    node *n = &pool[next++];
    n->v = 1;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) { n->v = -1; }

static inline __attribute__((noinline)) int64_t node_value(node *n) { return n->v; }

static int64_t (*stored_cb)(node *) = 0;
static node *stored_node = 0;

static inline __attribute__((noinline)) void store_cb(int64_t (*cb)(node *)) { stored_cb = cb; }
static inline __attribute__((noinline)) void store_node(node *n) { stored_node = n; }
static inline __attribute__((noinline)) int64_t call_stored(void) { return stored_cb(stored_node); }

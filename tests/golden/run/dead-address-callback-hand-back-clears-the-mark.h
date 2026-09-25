/* A C library that makes a node of its own and hands it to a callback: the
 * shape of a verify callback's X509_STORE_CTX or a json_c_visit node (the
 * ffi-pragmatist's `cb_reuse` at panel 177). The pool's free list hands
 * `visit` the address the program just gave back, deterministically. */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v;
    node *next_free;
};

static node node_pool[8];
static node *node_free_list = 0;
static int64_t node_next_fresh = 0;

static inline __attribute__((noinline)) node *node_new(void) {
    node *n;
    if (node_free_list != 0) {
        n = node_free_list;
        node_free_list = n->next_free;
    } else {
        n = &node_pool[node_next_fresh++];
    }
    n->v = 1;
    n->next_free = 0;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) {
    n->v = -1;
    n->next_free = node_free_list;
    node_free_list = n;
}

static inline __attribute__((noinline)) int64_t node_value(node *n) { return n->v; }

static inline __attribute__((noinline)) int64_t visit(int64_t (*cb)(node *)) {
    node *inner = node_new();
    inner->v = 42;
    int64_t r = cb(inner);
    node_free(inner);
    return r;
}

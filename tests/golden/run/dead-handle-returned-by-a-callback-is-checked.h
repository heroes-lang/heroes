/* A C library that asks a callback for a node and WRITES 64 bytes into it
 * (panel 177's critic, `cb_return_poison64`). The pool is static: a write
 * into a freed node would land in the pool and exit 0, which is the silent
 * wrong answer this case exists to refuse. */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v[8];
};

static inline __attribute__((noinline)) node *node_new(void) {
    static node pool[4];
    static int64_t next = 0;
    node *n = &pool[next++];
    n->v[0] = 1;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) { n->v[0] = -1; }

static inline __attribute__((noinline)) int64_t ask(node *(*cb)(int32_t)) {
    node *n = cb(3);
    for (int i = 0; i < 8; i++) n->v[i] = 0x4141414141414141L;
    return n->v[7];
}

/* A C library that asks a callback for a pair of nodes and writes through
 * both (the landing's skeptic, probe `e8`, 2026-09-25). Static pool. */
#include <stdint.h>

typedef struct node node;

struct node {
    int64_t v;
};

typedef struct pair pair;

struct pair {
    node *a;
    node *b;
};

static inline __attribute__((noinline)) node *node_new(void) {
    static node pool[4];
    static int64_t next = 0;
    node *n = &pool[next++];
    n->v = 1;
    return n;
}

static inline __attribute__((noinline)) void node_free(node *n) { n->v = -1; }

static inline __attribute__((noinline)) int64_t ask_pair(pair (*cb)(int32_t)) {
    pair p = cb(1);
    p.a->v = 5;
    p.b->v = 6;
    return p.a->v * 10 + p.b->v;
}

static inline __attribute__((noinline)) int64_t one(void) { return 1; }

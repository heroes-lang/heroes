/* Panel 261's R4 (f): a pool allocator whose free list hands the last address
 * given back out first (the ffi-pragmatist's `reuse/shapes.h`), a driver
 * handing C's own fresh node to a callback by value, and a C function calling
 * a callback once. */
#ifndef CALLBACK_THROUGH_A_VALUE_H
#define CALLBACK_THROUGH_A_VALUE_H
#include <stdint.h>
typedef struct node node;
struct node { int64_t v[8]; };
static node pool_mem[8];
static node *pool_free_list[8];
static int pool_next = 0, pool_nfree = 0;
static inline __attribute__((noinline)) node *node_new(void) {
    node *n = pool_nfree > 0 ? pool_free_list[--pool_nfree] : &pool_mem[pool_next++];
    for (int i = 0; i < 8; i++) n->v[i] = 0;
    n->v[0] = 1;
    return n;
}
static inline __attribute__((noinline)) void node_free(node *n) { n->v[0] = -1; pool_free_list[pool_nfree++] = n; }
static inline __attribute__((noinline)) int64_t node_peek(node *n) { return n->v[0]; }
static inline __attribute__((noinline)) int64_t hand_fresh(int32_t (*cb)(node *)) {
    node *n = node_new();
    int32_t r = cb(n);
    return n->v[0] * 10 + r;
}
static inline int64_t call_val(int64_t (*cb)(int64_t)) { return cb(41); }
#endif

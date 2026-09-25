/* An adder that TAKES the child only when it succeeds (0) and leaves it with
 * the caller when it fails (-1), json-c's shape. A pool instead of malloc, so
 * the case counts on every leg without an allocator. */
#include <stdint.h>
typedef struct node { int64_t v; struct node *child; } node;
static inline __attribute__((noinline)) node *node_new(int64_t v) { static node pool[8]; static int next = 0; pool[next].v = v; pool[next].child = 0; return &pool[next++]; }
static inline __attribute__((noinline)) int32_t node_add(node *parent, node *child, int32_t fail) { if (fail) return -1; parent->child = child; return 0; }
static inline __attribute__((noinline)) void node_put(node *n) { if (n->child) n->child = 0; }
static inline __attribute__((noinline)) int64_t node_sum(node *n) { return n->child ? n->v + n->child->v : n->v; }

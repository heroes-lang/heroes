/* The same adder as the case beside it; the program forgets the child when
 * the add fails. */
#include <stdint.h>
typedef struct node { int64_t v; struct node *child; } node;
static inline __attribute__((noinline)) node *node_new(int64_t v) { static node pool[8]; static int next = 0; pool[next].v = v; pool[next].child = 0; return &pool[next++]; }
static inline __attribute__((noinline)) int32_t node_add(node *parent, node *child, int32_t fail) { if (fail) return -1; parent->child = child; return 0; }
static inline __attribute__((noinline)) void node_put(node *n) { if (n->child) n->child = 0; }

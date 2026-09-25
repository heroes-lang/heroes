/* Same-size nodes from a static pool, so nothing here depends on an allocator
 * and the case can assert what the program did rather than a crash.
 * noinline so each call stays a call. */
#include <stdint.h>
typedef struct node { int64_t v; } node;
static inline __attribute__((noinline)) node *node_new(int64_t v) { static node pool[8]; static int next = 0; pool[next].v = v; return &pool[next++]; }
static inline __attribute__((noinline)) void node_free(node *n) { (void)n; }
static inline __attribute__((noinline)) int64_t node_peek(node *n) { return n->v; }

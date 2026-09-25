/* Same-size nodes from a pool, the shape the llm-ergonomist named at panel
 * 176 (`docs/panel/177-briefs/node.h`), over a static pool here so nothing
 * in it depends on an allocator. `check` never opens it. */
#include <stdint.h>
typedef struct node { int64_t v; } node;
static inline __attribute__((noinline)) node *node_new(void) { static node pool[8]; static int next = 0; pool[next].v = next; return &pool[next++]; }
static inline __attribute__((noinline)) void node_free(node *n) { (void)n; }
static inline __attribute__((noinline)) int64_t node_peek(node *n) { return n->v; }

/* Panel 215, the ffi-pragmatist's build of route C: <sys/queue.h>'s TAILQ_HEAD,
   whose tqh_last points into the head itself while the list is empty. */
#include <sys/queue.h>
#include <stdint.h>
struct item { int64_t v; TAILQ_ENTRY(item) link; };
TAILQ_HEAD(itemq, item);
struct qbox { struct itemq q; int64_t n; };
static struct item tq_items[4];
static inline void q_init(struct qbox *b) { TAILQ_INIT(&b->q); b->n = 0; }
static inline void q_push(struct qbox *b, int64_t v) { tq_items[b->n].v = v; TAILQ_INSERT_TAIL(&b->q, &tq_items[b->n], link); b->n++; }
static inline int64_t q_first(struct qbox *b) { return TAILQ_EMPTY(&b->q) ? -1 : TAILQ_FIRST(&b->q)->v; }

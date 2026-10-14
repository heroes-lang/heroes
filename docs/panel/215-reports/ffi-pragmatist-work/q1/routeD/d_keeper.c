/* ffi-pragmatist, panel 215, route D over a record whose address C keeps: <sys/queue.h>'s TAILQ,
   reached through static inline functions as design.md §4.19 reaches a function-like macro. */
#include <sys/queue.h>
#include <stdio.h>
#include <string.h>
struct item { int v; TAILQ_ENTRY(item) link; };
TAILQ_HEAD(tq, item);
static inline void tq_init(struct tq *h) { TAILQ_INIT(h); }
static inline void tq_push(struct tq *h, struct item *it) { TAILQ_INSERT_TAIL(h, it, link); }
static inline int tq_first(const struct tq *h) { return TAILQ_EMPTY(h) ? -1 : TAILQ_FIRST(h)->v; }
/* route D: a temporary zeroed whole, the record's two fields copied in, C called, copied back */
static struct tq *route_d(struct tq *z, const struct tq *s) { memset(z, 0, sizeof *z); z->tqh_first = s->tqh_first; z->tqh_last = s->tqh_last; return z; }
int main(void) {
    struct tq h; struct item a = {7, {0}};
    tq_init(&h); tq_push(&h, &a); printf("today   first %d\n", tq_first(&h));
    struct tq g = {0}, t1, t2, t3; struct item b = {7, {0}}, c = {8, {0}};
    tq_init(route_d(&t1, &g)); g = t1;            /* copy back: g.tqh_last now points into t1 */
    tq_push(route_d(&t2, &g), &b); g = t2;
    tq_push(route_d(&t3, &g), &c); g = t3;
    printf("route D first %d, tqh_last %s\n", tq_first(&g), g.tqh_last == &g.tqh_first ? "into the record" : "into the temporary");
    return 0;
}

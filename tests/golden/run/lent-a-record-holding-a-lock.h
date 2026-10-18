/* Panel 217's task C (M-readonly-lend): a struct holding a POSIX mutex, which
   no copy may hold, read by C through a pointer to const. The Windows box has
   no pthread.h, so the case is judged on Darwin and Linux and skipped there
   for the header it lacks. */
#include <pthread.h>
#include <stdint.h>

struct counter { pthread_mutex_t lock; int64_t n; };

static inline int32_t counter_init(struct counter *c) { c->n = 0; return pthread_mutex_init(&c->lock, 0); }
static inline int64_t counter_value(const struct counter *c) { return c->n; }

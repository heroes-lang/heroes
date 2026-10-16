/* Panel 215's R2 (issue 1335): a struct holding a POSIX mutex, reached by C
   only through its place. Each shim acts on the mutex inside the place it is
   lent; `counter_made` is C returning one by value, a fresh value C made.
   The Windows box has no pthread.h, so the case is judged on Darwin and
   Linux and skipped there by name. */
#include <pthread.h>
#include <stdint.h>

struct counter { pthread_mutex_t lock; int64_t n; };

static inline int32_t counter_init(struct counter *c) { return pthread_mutex_init(&c->lock, 0); }
static inline int32_t counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }
static inline int32_t counter_unlock(struct counter *c) { return pthread_mutex_unlock(&c->lock); }
static inline int32_t add_to(int64_t *n) { *n += 1; return 0; }
static inline struct counter counter_made(void) { struct counter c = { PTHREAD_MUTEX_INITIALIZER, 41 }; return c; }

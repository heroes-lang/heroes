/* Panel 215: a header that returns and takes a lock-holding struct by value. */
#include <pthread.h>
#include <stdint.h>
struct counter { pthread_mutex_t lock; int64_t n; };
static inline struct counter counter_make(void) { struct counter c = { PTHREAD_MUTEX_INITIALIZER, 41 }; return c; }
static inline int64_t counter_peek(struct counter c) { return c.n; }
static inline int counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }

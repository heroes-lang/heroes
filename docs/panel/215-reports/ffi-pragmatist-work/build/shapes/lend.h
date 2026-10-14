#include <pthread.h>
#include <stdint.h>
struct counter { pthread_mutex_t lock; int64_t n; };
static inline int counter_init(struct counter *c) { return pthread_mutex_init(&c->lock, 0); }
static inline int counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }
static inline int counter_unlock(struct counter *c) { return pthread_mutex_unlock(&c->lock); }
static inline int add_to(int64_t *n) { *n += 1; return 0; }

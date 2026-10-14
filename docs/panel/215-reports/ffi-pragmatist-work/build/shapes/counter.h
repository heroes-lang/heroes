/* The critic's stand-in for the experiment's counter.h, panel 215 pass 1. */
#include <pthread.h>
struct counter { pthread_mutex_t lock; long n; };
static inline int counter_init(struct counter *c) { c->n = 0; return pthread_mutex_init(&c->lock, NULL); }
static inline int counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }
static inline int counter_unlock(struct counter *c) { return pthread_mutex_unlock(&c->lock); }
static inline long counter_value(const struct counter *c) { return c->n; }

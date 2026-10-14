/* Critic, panel 215 pass 2: a C thread contends for the lock of a counter
   that lives in a Heroes local (lent with @, C keeping the address while
   the place does not move), while Heroes reads its n under that lock. */
#include <pthread.h>
#include <stdint.h>
struct counter { pthread_mutex_t lock; int64_t n; };
static void *race_spin(void *p) {
    struct counter *c = p;
    for (int i = 0; i < 200000; i++) { pthread_mutex_lock(&c->lock); c->n++; pthread_mutex_unlock(&c->lock); }
    return 0;
}
static pthread_t race_th;
static inline int counter_init(struct counter *c) { c->n = 0; return pthread_mutex_init(&c->lock, 0); }
static inline int race_start(struct counter *c) { return pthread_create(&race_th, 0, race_spin, c); }
static inline int race_join(struct counter *c) { (void)c; return pthread_join(race_th, 0); }
static inline int counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }
static inline int counter_unlock(struct counter *c) { return pthread_mutex_unlock(&c->lock); }

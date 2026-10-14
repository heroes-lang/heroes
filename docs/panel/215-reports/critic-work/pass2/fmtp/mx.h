/* Compiler-engineer, panel 215: issue 1335's measurement. A struct holding a
   mutex by value, the shape a program binds (panel 215's Task B); each shim
   acts on the mutex inside the place it is lent. */
#include <pthread.h>
#include <stddef.h>
struct box { pthread_mutex_t m; long n; };
static inline int box_init(struct box *b) { return pthread_mutex_init(&b->m, NULL); }
static inline int box_lock(struct box *b) { return pthread_mutex_lock(&b->m); }
static inline int box_trylock(struct box *b) { return pthread_mutex_trylock(&b->m); }
static inline int box_unlock(struct box *b) { return pthread_mutex_unlock(&b->m); }
static inline int box_destroy(struct box *b) { return pthread_mutex_destroy(&b->m); }

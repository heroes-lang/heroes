/* Panel 215's R2 (issue 1335): a struct holding a POSIX mutex by value,
   which C forbids to copy (POSIX XSH 2.9.9: "The effect of referring to a
   copy of the object when locking, unlocking, or destroying it is
   undefined"). Each shim acts on the mutex inside the place it is lent.
   The Windows box has no pthread.h, so these cases are judged on Darwin and
   Linux and skipped there by name. */
#include <pthread.h>
#include <stdint.h>

struct box { pthread_mutex_t m; int64_t n; };

static inline int32_t box_init(struct box *b) { return pthread_mutex_init(&b->m, 0); }
static inline int32_t box_lock(struct box *b) { return pthread_mutex_lock(&b->m); }
static inline int32_t box_unlock(struct box *b) { return pthread_mutex_unlock(&b->m); }
static inline int64_t box_peek(struct box b) { return b.n; }

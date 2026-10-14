/* Panel 215: glibc's pthread_spinlock_t, a typedef of volatile int. */
#include <pthread.h>
#include <stdint.h>
struct sbox { pthread_spinlock_t s; int64_t n; };
static inline int sp_init(struct sbox *b) { return pthread_spin_init(&b->s, PTHREAD_PROCESS_PRIVATE); }
static inline int sp_lock(struct sbox *b) { return pthread_spin_lock(&b->s); }
static inline int sp_try(struct sbox *b) { return pthread_spin_trylock(&b->s); }

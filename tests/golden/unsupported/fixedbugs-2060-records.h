/* Defect 2060, 2026-10-14: a typedef of a tagged struct with three
 * functions taking it, as this Mac's `pthread_mutex_t` is. */
#include <stdint.h>
typedef struct lock_s { int32_t state; } lock_t;
static inline int32_t lock_init(lock_t *l) { l->state = 0; return 0; }
static inline int32_t lock_take(lock_t *l) { l->state = 1; return 0; }
static inline int32_t lock_give(lock_t *l) { l->state = 0; return 0; }

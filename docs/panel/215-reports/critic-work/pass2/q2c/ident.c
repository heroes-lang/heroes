/* Critic, panel 215 pass 2: route C asked of clang by TYPE IDENTITY against
   POSIX's own typedef names, given only member names (which the dump gives),
   rather than by the spelling the dump prints. */
#include <pthread.h>
#include <stdint.h>
struct counter { pthread_mutex_t lock; int64_t n; };
struct outer { int32_t k; struct counter inner; };
#define IS(T, M, U) __builtin_types_compatible_p(__typeof__(((T *)0)->M), U)
_Static_assert(IS(struct counter, lock, pthread_mutex_t), "lock is a pthread_mutex_t");
_Static_assert(!IS(struct counter, n, pthread_mutex_t), "n is not");
_Static_assert(IS(struct outer, inner.lock, pthread_mutex_t), "nested lock found by its path");
_Static_assert(!IS(struct counter, lock, pthread_cond_t), "a mutex is not a cond");
int main(void) { return 0; }

/* ffi-pragmatist, panel 215: what clang's record-layout dump says of a struct holding a lock, and
   of a self-referential one, the input a header-decided route would read. */
#include <pthread.h>
#include <sys/queue.h>
struct counter { pthread_mutex_t lock; long n; };
struct item { int v; TAILQ_ENTRY(item) link; };
TAILQ_HEAD(tq, item);
struct counter c; struct tq q;
int sizes[] = { sizeof(struct counter), sizeof(struct tq) };

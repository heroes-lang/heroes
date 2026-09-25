/* ONE pool of nodes with a free list under a lock, shared by every thread:
 * what every real allocator is. A node one thread gives back is the next node
 * the other thread is handed, often while the first thread's `sn_free` has
 * returned to C's caller and not yet to the runtime (defect 098). The lock is
 * the platform's own, because Windows has no pthreads. */
#include <stdint.h>
#if defined(_WIN32)
#include <windows.h>
static SRWLOCK sn_lock = SRWLOCK_INIT;
#define sn_take() AcquireSRWLockExclusive(&sn_lock)
#define sn_drop() ReleaseSRWLockExclusive(&sn_lock)
#else
#include <pthread.h>
static pthread_mutex_t sn_lock = PTHREAD_MUTEX_INITIALIZER;
#define sn_take() pthread_mutex_lock(&sn_lock)
#define sn_drop() pthread_mutex_unlock(&sn_lock)
#endif

typedef struct sn sn;

struct sn {
    int64_t v;
    sn *next_free;
};

static sn sn_pool[64];
static sn *sn_free_list = 0;
static int64_t sn_next_fresh = 0;

static inline __attribute__((noinline)) sn *sn_new(void) {
    sn_take();
    sn *n;
    if (sn_free_list != 0) {
        n = sn_free_list;
        sn_free_list = n->next_free;
    } else {
        n = &sn_pool[sn_next_fresh++ % 64];
    }
    n->v = 1;
    n->next_free = 0;
    sn_drop();
    return n;
}

static inline __attribute__((noinline)) void sn_free(sn *n) {
    sn_take();
    n->v = -1;
    n->next_free = sn_free_list;
    sn_free_list = n;
    sn_drop();
}

static inline __attribute__((noinline)) int64_t sn_value(sn *n) { return n->v; }

/* Critic, panel 178: is the header's own initialiser a C expression of its type
   (a compound literal), on every leg, for every *_INITIALIZER the census headers define? */
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#define TRY(T, M) do { T v = (T)M; T z; memset(&z, 0, sizeof z); printf("%-40s zero-equal=%d\n", #M, memcmp(&v, &z, sizeof v) == 0); } while (0)
int main(void) {
    TRY(pthread_mutex_t, PTHREAD_MUTEX_INITIALIZER);
    TRY(pthread_cond_t, PTHREAD_COND_INITIALIZER);
    TRY(pthread_rwlock_t, PTHREAD_RWLOCK_INITIALIZER);
#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    TRY(pthread_mutex_t, PTHREAD_RECURSIVE_MUTEX_INITIALIZER);
#endif
#ifdef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
    TRY(pthread_mutex_t, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP);
#endif
#ifdef PTHREAD_ONCE_INIT
    TRY(pthread_once_t, PTHREAD_ONCE_INIT);
#endif
    return 0;
}

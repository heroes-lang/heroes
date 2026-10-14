#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#if __has_include(<threads.h>)
#define HAS_THREADS 1
#else
#define HAS_THREADS 0
#endif
#define IS(A, B) __builtin_types_compatible_p(A, B)
struct plain { int n; volatile int v; int32_t w; };
int main(void) {
  printf("_POSIX_SEMAPHORES %ld _POSIX_SPIN_LOCKS %ld _POSIX_BARRIERS %ld\n",
#ifdef _POSIX_SEMAPHORES
   (long)_POSIX_SEMAPHORES,
#else
   -99L,
#endif
#ifdef _POSIX_SPIN_LOCKS
   (long)_POSIX_SPIN_LOCKS,
#else
   -99L,
#endif
#ifdef _POSIX_BARRIERS
   (long)_POSIX_BARRIERS
#else
   -99L
#endif
  );
  printf("int~sem_t %d\n", IS(int, sem_t));
  printf("plain.n~sem_t %d\n", IS(__typeof__(((struct plain*)0)->n), sem_t));
#if defined(_POSIX_SPIN_LOCKS) && _POSIX_SPIN_LOCKS > 0
  printf("int~spin %d volatile-int~spin %d int32~spin %d\n", IS(int, pthread_spinlock_t), IS(volatile int, pthread_spinlock_t), IS(int32_t, pthread_spinlock_t));
#else
  printf("no spinlock type\n");
#endif
  printf("SEM_FAILED %s PTHREAD_MUTEX_INITIALIZER defined\n",
#ifdef SEM_FAILED
  "defined,"
#else
  "absent,"
#endif
  );
  printf("__STDC_NO_THREADS__ %s, has threads.h %d\n",
#ifdef __STDC_NO_THREADS__
  "defined",
#else
  "absent",
#endif
  HAS_THREADS);
  return 0;
}

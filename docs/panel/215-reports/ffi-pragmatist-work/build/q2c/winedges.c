#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#if __has_include(<threads.h>)
#include <threads.h>
#define HAS_THREADS 1
#else
#define HAS_THREADS 0
#endif
#define IS(A, B) __builtin_types_compatible_p(A, B)
#define CLS(T) __builtin_classify_type(*(T *)0)
struct box { CRITICAL_SECTION cs; SRWLOCK l; CONDITION_VARIABLE cv; int32_t n; void *p; };
int main(void) {
  printf("classify CS %d SRW %d CV %d int %d\n", CLS(CRITICAL_SECTION), CLS(SRWLOCK), CLS(CONDITION_VARIABLE), CLS(int));
  printf("box.n~SRW %d box.p~SRW %d box.l~SRW %d box.cs~CS %d box.cv~CV %d RTL_SRW~SRW %d\n",
    IS(__typeof__(((struct box*)0)->n), SRWLOCK), IS(__typeof__(((struct box*)0)->p), SRWLOCK),
    IS(__typeof__(((struct box*)0)->l), SRWLOCK), IS(__typeof__(((struct box*)0)->cs), CRITICAL_SECTION),
    IS(__typeof__(((struct box*)0)->cv), CONDITION_VARIABLE), IS(RTL_SRWLOCK, SRWLOCK));
  printf("SRW~CV %d (both one pointer)\n", IS(SRWLOCK, CONDITION_VARIABLE));
#ifdef _WINNT_
  printf("_WINNT_ defined\n");
#endif
  printf("threads.h %d\n", HAS_THREADS);
#if HAS_THREADS
  printf("classify mtx_t %d cnd_t %d; int~mtx_t %d\n", CLS(mtx_t), CLS(cnd_t), IS(int, mtx_t));
#endif
  return 0;
}

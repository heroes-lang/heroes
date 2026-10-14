#include <pthread.h>
#include <stdint.h>
struct counter { pthread_mutex_t lock; int64_t n; };
struct outer { int32_t k; struct counter inner; };
struct arr { int32_t k; struct counter many[2]; };
struct anon { int32_t k; union { pthread_mutex_t m; int64_t x; }; };
struct cond { pthread_cond_t cv; int32_t k; };
struct plain { int32_t a; double d; };

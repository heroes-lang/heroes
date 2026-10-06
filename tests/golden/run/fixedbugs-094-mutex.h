/* Defect 094, lane ffi13, 2026-10-06: the system's own mutex initialiser,
   inside a brace list of the program's own, so one binding runs on every
   platform that has `pthread.h` (Darwin and Linux; the Windows box's C library
   has none, and the case is skipped there by name). */
#include <pthread.h>
#include <stdint.h>

typedef struct { int32_t id; pthread_mutex_t m; } hero_mutex;

#define HERO_MUTEX_INIT { 7, PTHREAD_MUTEX_INITIALIZER }

static inline int32_t hero_lock(hero_mutex *h) { return pthread_mutex_lock(&h->m); }
static inline int32_t hero_unlock(hero_mutex *h) { return pthread_mutex_unlock(&h->m); }

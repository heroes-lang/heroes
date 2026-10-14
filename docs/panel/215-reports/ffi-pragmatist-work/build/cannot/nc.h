/* ffi-pragmatist, panel 215: C types C or POSIX says must not be copied, and the functions a
   binding reaches them through (function-like macros through static inline, design.md §4.19). */
#include <pthread.h>
#include <errno.h>
#include <zlib.h>
#include <string.h>
struct counter { pthread_mutex_t lock; long n; };      /* the shape a program meets: a struct HOLDING one */
static inline int zs_init(z_stream *s) { memset(s, 0, sizeof *s); return deflateInit(s, Z_DEFAULT_COMPRESSION); }
static inline int zs_finish(z_stream *s) { unsigned char out[64]; s->next_in = (unsigned char *)"abc"; s->avail_in = 3; s->next_out = out; s->avail_out = sizeof out; return deflate(s, Z_FINISH); }
static inline int counter_init(struct counter *c) { c->n = 0; return pthread_mutex_init(&c->lock, NULL); }
static inline int counter_lock(struct counter *c) { return pthread_mutex_lock(&c->lock); }
static inline int counter_trylock(struct counter *c) { return pthread_mutex_trylock(&c->lock); }
static inline int counter_unlock(struct counter *c) { return pthread_mutex_unlock(&c->lock); }
static inline int counter_destroy(struct counter *c) { return pthread_mutex_destroy(&c->lock); }

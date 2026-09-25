#include <stdint.h>
typedef struct hh { int64_t open; } hh;
static inline __attribute__((noinline)) hh *h_open(void) { static hh one; one.open = 1; return &one; }
static inline __attribute__((noinline)) void h_close(hh *x) { x->open = 0; }
static inline __attribute__((noinline)) void h_take(hh *x) { x->open = 0; }
static inline __attribute__((noinline)) void h_eat(hh **p) { (*p)->open = 0; }
static inline __attribute__((noinline)) void h_give(hh **p, hh *keep) { keep->open = (*p)->open; }
static inline __attribute__((noinline)) int32_t h_out(hh **p) { *p = h_open(); return 0; }

#include <stdint.h>
typedef struct hh { int64_t refs; struct hh *child; } hh;
static inline __attribute__((noinline)) hh *h_open(void) { static hh pool[4]; static int next; hh *one = &pool[next++ % 4]; one->refs = 1; one->child = 0; return one; }
static inline __attribute__((noinline)) void h_close(hh *x) { if (x->child) x->child->refs--; x->refs--; }
static inline __attribute__((noinline)) void h_put(hh *parent, hh *child) { parent->child = child; }

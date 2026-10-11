#include <stdint.h>
typedef struct hh { int64_t refs; struct hh *child; } hh;
static inline __attribute__((noinline)) hh *h_open(void) { static hh pool[8]; static int next; hh *one = &pool[next++ % 8]; one->refs = 1; one->child = 0; return one; }
static inline __attribute__((noinline)) void h_close(hh *x) { x->refs--; }
static inline __attribute__((noinline)) int64_t h_try_close(hh *x) { x->refs--; return 0; }
static inline __attribute__((noinline)) void h_ref(hh *x) { x->refs++; }
static inline __attribute__((noinline)) hh *h_child(hh *x) { return x->child; }
static inline __attribute__((noinline)) int64_t h_peek(hh *x) { return x->refs; }

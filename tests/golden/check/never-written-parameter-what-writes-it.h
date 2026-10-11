#include <stdint.h>
typedef struct hh { int64_t refs; } hh;
static inline __attribute__((noinline)) hh *h_open(void) { static hh pool[8]; static int next; hh *one = &pool[next++ % 8]; one->refs = 1; return one; }
static inline __attribute__((noinline)) void h_close(hh *x) { x->refs--; }
static inline __attribute__((noinline)) void h_into(hh *child, hh *parent) { (void)child; (void)parent; }
static inline __attribute__((noinline)) void put_seven(int64_t *out) { *out = 7; }
static inline __attribute__((noinline)) void slot_fill(void *p, int64_t n) { (void)p; (void)n; }

#include <stdint.h>
typedef struct hh { int64_t refs; } hh;
static inline __attribute__((noinline)) hh *h_open(void) { static hh one; one.refs = 1; return &one; }
static inline __attribute__((noinline)) void h_close(hh *x) { x->refs--; }
static inline __attribute__((noinline)) void h_wrap(hh *x, hh *keep) { keep->refs += x->refs; }
static inline __attribute__((noinline)) void h_ref(hh *x) { x->refs++; }
static inline __attribute__((noinline)) hh *h_get(hh *x) { x->refs++; return x; }

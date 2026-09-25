#include <stdint.h>
typedef struct fl { int64_t open; } fl;
typedef struct wr { fl *inner; } wr;
static inline __attribute__((noinline)) fl *fl_open(void) { static fl one; one.open = 1; return &one; }
static inline __attribute__((noinline)) void fl_close(fl *f) { f->open = 0; }
static inline __attribute__((noinline)) void fl_both(fl *a, fl *b) { a->open = 0; b->open = 0; }
static inline __attribute__((noinline)) void fl_gone(fl *a, fl *b) { a->open = 0; b->open = 0; }
static inline __attribute__((noinline)) void fl_keep(fl *a, fl *b) { b->open = a->open; }
static inline __attribute__((noinline)) void fl_cell(fl *a, fl **b) { *b = a; }
static inline __attribute__((noinline)) wr *fl_wrap(fl *a) { static wr one; one.inner = a; return &one; }
static inline __attribute__((noinline)) void wr_free(wr *w) { w->inner = 0; }

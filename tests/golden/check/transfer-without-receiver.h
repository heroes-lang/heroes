#include <stdint.h>
typedef struct fl { int64_t open; } fl;
static inline __attribute__((noinline)) fl *fl_popen(void) { static fl one; one.open = 1; return &one; }
static inline __attribute__((noinline)) void fl_pclose(fl *f) { f->open = 0; }
static inline __attribute__((noinline)) void fl_fclose(fl *f) { f->open = 0; }

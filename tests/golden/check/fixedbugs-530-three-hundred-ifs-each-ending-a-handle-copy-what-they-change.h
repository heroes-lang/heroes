/* Defect 530's case, the shape of defect 499's: one static cell handed out
 * every time and a release that frees nothing. `check` never opens it. */
#include <stdint.h>
typedef struct hh hh;
static char hero_fixed_cell[8];
static inline __attribute__((noinline)) hh *f_open(void) { return (hh *)hero_fixed_cell; }
static inline __attribute__((noinline)) void f_close(hh *x) { (void)x; }
static inline __attribute__((noinline)) int64_t f_peek(hh *x) { (void)x; return 0; }

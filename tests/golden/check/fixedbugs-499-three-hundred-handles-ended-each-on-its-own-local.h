/* Defect 499's case, the shape of defect 409's: one static cell handed out
 * every time and a release that frees nothing. `check` never opens it. */
#include <stdlib.h>
typedef struct hh hh;
typedef struct pp { hh *left; hh *right; } pp;
static char hero_fixed_cell[8];
static inline __attribute__((noinline)) hh *f_open(void) { return (hh *)hero_fixed_cell; }
static inline __attribute__((noinline)) void f_close(hh *x) { (void)x; }
static inline __attribute__((noinline)) void f_keep(hh *x) { (void)x; }
static inline __attribute__((noinline)) pp *p_open(void) { static pp one; return &one; }

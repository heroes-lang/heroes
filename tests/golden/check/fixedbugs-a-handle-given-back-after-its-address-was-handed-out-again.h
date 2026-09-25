/* Panel 175's completeness critic (`u1_static.hero`): one address handed out
 * again, and C's allocator is not the only thing that reuses one. `f_open`
 * returns one static cell every time and `f_close` frees nothing, so the
 * shape does not depend on libmalloc's mood, which two byte-identical
 * binaries of the malloc version disagreed about (defect 077's filing). */
#include <stdlib.h>
typedef struct hh hh;
static char hero_fixed_cell[8];
static inline __attribute__((noinline)) hh *f_open(void) { return (hh *)hero_fixed_cell; }
static inline __attribute__((noinline)) void f_close(hh *x) { (void)x; }

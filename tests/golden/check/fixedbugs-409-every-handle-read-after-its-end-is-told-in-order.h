/* Defect 409's case: one static cell handed out every time and a release
 * that frees nothing, the shape of
 * `fixedbugs-a-handle-given-back-after-its-address-was-handed-out-again.h`.
 * `check` never opens it. */
#include <stdlib.h>
typedef struct hh hh;
static char hero_fixed_cell[8];
static inline __attribute__((noinline)) hh *f_open(void) { return (hh *)hero_fixed_cell; }
static inline __attribute__((noinline)) void f_close(hh *x) { (void)x; }

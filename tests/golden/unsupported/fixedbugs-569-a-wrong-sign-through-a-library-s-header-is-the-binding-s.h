/* Defect 569, 2026-10-10: a library header with a sign conversion in its own code. */
#include <stdint.h>
/* The library's own inline code, as `gmp.h` line 1882 writes it: a signed
   value returned as unsigned, a sign conversion C makes in silence. */
static inline unsigned magnitude(int x) { return x >= 0 ? x : -x; }
static inline int twice(int x) { return 2 * x; }

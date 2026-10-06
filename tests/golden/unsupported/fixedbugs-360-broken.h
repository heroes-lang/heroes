/* Defect 360, lane cli12, 2026-10-05: a header clang refuses on its own, a
   declaration with no value after its `=`. */
#include <stdint.h>
static inline int64_t twice(int64_t x) { return 2 * x; }
int broken = ;

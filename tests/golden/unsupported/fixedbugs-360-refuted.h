/* Defect 360, lane cli12, 2026-10-05: a header that does not compile, and
   whose declaration the program's extern also refutes. */
#include <stdint.h>
int unfinished = ;
static inline int64_t six(void) { return 6; }

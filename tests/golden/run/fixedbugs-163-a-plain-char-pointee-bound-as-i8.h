/* Defect 163, lane h158, 2026-10-02: C writes a plain `char` through the
   pointer it is handed. The program's compile reads that `char` as signed on
   every leg, under the compiler's `-fsigned-char` (panel 161). */
#include <stdint.h>
static inline void fill(char *p) { *p = (char)0xFF; }

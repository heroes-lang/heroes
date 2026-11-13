/* Defect 2495, lane b34-fsl, 2026-11-13: two inline bodies calling a
   function nothing on the link line defines. */
#include <stdint.h>
int base2495(void);
static inline int64_t top2495_a(void) { return base2495(); }
static inline int64_t top2495_b(void) { return base2495() + 1; }

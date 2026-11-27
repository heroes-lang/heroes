/* Defect 2877, lane win-longname, 2026-11-27: two inline bodies calling a
   function nothing on the link line defines, each bound by a module whose
   name takes 180 letters. */
#include <stdint.h>
int base2877(void);
static inline int64_t top2877_a(void) { return base2877(); }
static inline int64_t top2877_b(void) { return base2877() + 1; }

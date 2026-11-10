/* A package's float, bound through a function its own header defines, which
   a fetched build reads with nothing run (defect 2458). */
#ifndef PK2458_H
#define PK2458_H
#define PK_HALF 0.5
static inline double pk_half(void) { return PK_HALF; }
#endif

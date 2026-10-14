/* Defect 2056, 2026-10-14: a header that makes `fabs`, a name `<math.h>`
 * declares, a macro of its own function, and binds it itself. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define fabs my_fabs

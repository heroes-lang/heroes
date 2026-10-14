/* Defect 2059, 2026-10-14: a header making `fabs`, a function of `<math.h>`
 * the guard holds, a macro of its own, and `absval` a macro of `fabs`. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define fabs my_fabs
#define absval fabs

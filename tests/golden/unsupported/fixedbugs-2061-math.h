/* Defect 2061, 2026-10-14: a bound macro naming a macro spelled as a
 * function of <math.h>, which the groups' headers never declare. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define fabs(x) my_fabs(x)
#define absval fabs

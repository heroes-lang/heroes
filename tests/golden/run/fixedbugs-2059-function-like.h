/* Defect 2059, 2026-10-14: a function-like `absval` calling `fabs`, which
 * is `my_fabs`; the function the probe holds the binding to stands beside it. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
static inline double absval(double x) { return x; }
#define fabs my_fabs
#define absval(x) fabs(x)

/* Defect 2061, 2026-10-14: a bound macro naming a macro written with
 * parameters that has a function of its own name beside it, as libc's
 * `getc` does: the probe holds the binding to that function, and the call
 * expands the macro, as C reads the header. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
static inline double myabs(double x) { return x; }
#define myabs(x) my_fabs(x)
#define absval myabs

/* Defect 2056, 2026-10-14: the same as `fixedbugs-2056-shadow.h` with a
 * function-like macro, which a name written in parentheses does not expand
 * and a call does. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define fabs(x) my_fabs(x)
static inline int seven(void) { return 7; }

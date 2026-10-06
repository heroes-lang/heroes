/* Defect 361's other side, 2026-10-06: a header that makes `sqrt`, a name
 * <math.h> gives and the guard keeps, a macro of its own, which the program
 * binds. The binding is the header's, so the unit pushes the name again
 * before the guard's close and the macro stands: the program's `sqrt` doubles,
 * as this header says, rather than taking a root. */
static inline double doubled_root(double x) { return x * 2.0; }
#define sqrt(x) doubled_root(x)

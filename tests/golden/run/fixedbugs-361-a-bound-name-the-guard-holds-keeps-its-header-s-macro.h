/* Defect 361's other side, 2026-10-06: a header that makes `sqrt`, a name
 * <math.h> gives and the guard keeps, a macro of its own, which the program
 * binds. The binding is the header's, so the unit pushes the name again
 * before the guard's close and the macro stands: the program's `sqrt` doubles,
 * as this header says, rather than taking a root. */
/* Corrected 2026-10-10, defect 568 (panel 205's R3): this header named
 * <math.h>'s `sqrt` without including <math.h>, which the unit read before
 * the groups until then; it reads it after them now, so the header includes
 * what it names, and the case asks what it always asked. */
#include <math.h>
static inline double doubled_root(double x) { return x * 2.0; }
#define sqrt(x) doubled_root(x)

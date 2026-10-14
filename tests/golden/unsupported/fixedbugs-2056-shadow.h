/* Defect 2056, 2026-10-14: a header of the program's own that makes `fabs`,
 * a name `<math.h>` declares, a macro of its own function, which returns
 * 1000.0 whatever it is handed. A group binding `fabs` from `<math.h>` beside
 * it called this function at exit 0, in either order of the two groups. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define fabs my_fabs
static inline int seven(void) { return 7; }

/* Defect 2061, 2026-10-14: bound names whose macros name another, which
 * the probe's parenthesised call never expands or nothing declares. */
static inline double my_fabs(double x) { (void)x; return 1000.0; }
#define myabs(x) my_fabs(x)
#define absval myabs
#define mid myabs
#define chained mid
#define make_limit(x) ((x) * 2)
#define LIMIT make_limit
#define seven_now() 7
#define seven seven_now
#define nowhere nosuch_fn
#define NOWHERE nosuch_value

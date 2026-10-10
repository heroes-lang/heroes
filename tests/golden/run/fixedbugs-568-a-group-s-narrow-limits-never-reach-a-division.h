/* Defect 568 meeting defect 567, 2026-10-10 (lane b18-guard, at the round's
 * merge): a group's header that reads <stdint.h> and then gives the narrow
 * signed limits values of its own. The unit's division guard writes them at
 * the operands' width, so the guard must keep them and the unit define them
 * again after the groups, where the library's include guard keeps
 * <stdint.h> from defining them a second time. */
#include <stdint.h>
#undef INT8_MIN
#define INT8_MIN 0
#undef INT16_MIN
#define INT16_MIN 0
#undef INT32_MIN
#define INT32_MIN 0
static inline int narrow_limits_answer(void) { return 7; }

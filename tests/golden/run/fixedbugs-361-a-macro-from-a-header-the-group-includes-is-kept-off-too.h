/* Defect 361's shapes beside, 2026-10-06: the macros come from a header this
 * one includes, and take an object-like form on a runtime function, `bool`,
 * `true` and the runtime's string type. Each rewrote the program's own C
 * until the repair. */
#include <stdint.h>
#include "fixedbugs-361-a-macro-from-a-header-the-group-includes-is-kept-off-too-inner.h"
static inline int64_t thrice(int64_t x) { return 3 * x; }

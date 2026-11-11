/* Panel 245's R1, 2026-11-11: a header whose `static inline` function nobody
 * calls calls a function marked `warning` (the ffi-pragmatist's `unused-b`). */
#include <stdint.h>
__attribute__((warning("this call overruns its buffer"))) static inline int64_t warned(int64_t v) { return v; }
static inline int64_t never_called(int64_t x) { return warned(x); }
static inline int64_t door(int64_t x) { return x + 1; }

/* Panel 245's R1, 2026-11-11: a door calling a function its header marks
 * `warning`, which clang says at `-O0` and, the call inlined away, not at
 * `-O2` (the sitting critic's shape `attr_warning`). */
#include <stdint.h>
__attribute__((warning("this call overruns its buffer"))) static inline int64_t warned(int64_t v) { return v; }
static inline int64_t door(int64_t x) { return warned(x); }

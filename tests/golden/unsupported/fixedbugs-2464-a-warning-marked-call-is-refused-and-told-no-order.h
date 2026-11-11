/* Defect 2464, panel 245's R1, 2026-11-11: a door calling a function its
 * header marks `warning`, which clang says only while it generates code
 * (the sitting critic's shape `attr_warning`). */
#include <stdint.h>
__attribute__((warning("this call overruns its buffer"))) static inline int64_t warned(int64_t v) { return v; }
static inline int64_t door(int64_t x) { return warned(x); }

/* Defect 2464, panel 245's R1, 2026-11-11: a door calling a function its
 * header marks deprecated (the sitting critic's shape `deprecated`). */
#include <stdint.h>
__attribute__((deprecated("use another"))) static inline int64_t old(int64_t v) { return v; }
static inline int64_t door(int64_t x) { return old(x) + 1; }

/* Defect 2464, panel 245's R1, 2026-11-11: a door calling a function its
 * own header's `diagnose_if` warns of (the sitting critic's shape
 * `diagnose_if`). */
#include <stdint.h>
static inline int64_t checked(int64_t v) __attribute__((diagnose_if(v < 0, "a negative length overruns the buffer", "warning"))) { return v; }
static inline int64_t door(int64_t x) { return checked(-1) + x; }

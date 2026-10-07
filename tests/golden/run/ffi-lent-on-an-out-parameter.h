#ifndef FFI_LENT_ON_AN_OUT_PARAMETER_H
#define FFI_LENT_ON_AN_OUT_PARAMETER_H

/* Panel 196's R3 (lane b13-land-buf, 2026-10-07): out-parameters C writes
   during the call and whose address it keeps nothing of afterwards. */
#include <stdint.h>

struct span { int64_t lo; int64_t hi; };

static inline double halve(double x, int32_t *e) { *e = 1; return x / 2.0; }
static inline void widen(struct span *s) { s->lo -= 1; s->hi += 1; }

#endif

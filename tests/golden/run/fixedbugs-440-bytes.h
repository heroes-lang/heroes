#ifndef FIXEDBUGS_440_BYTES_H
#define FIXEDBUGS_440_BYTES_H

/* Defect 440's legal neighbour across the C boundary (lane b13-land-addr,
   2026-10-07): C handed one byte through `@`, which it increments. */
#include <stdint.h>

static inline void bump(uint8_t *n) { *n += 1; }

#endif

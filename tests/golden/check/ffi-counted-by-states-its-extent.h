#ifndef FFI_COUNTED_BY_STATES_ITS_EXTENT_H
#define FFI_COUNTED_BY_STATES_ITS_EXTENT_H

/* Panel 196's R5 (lane b13-land-buf, 2026-10-07): the shapes of a stated
   extent, judged where they are written. */
#include <stdint.h>

struct two { int32_t a; };

#define FILL_LEN 16
#define FILL_SCALE 1.5
#define LATE_LEN 16u

void fill_zero(void *p);
void fill_huge(void *p);
void fill_nowhere(void *p);
void fill_other(void *p);
void fill_float(void *p);
void fill_by_value(void *p);
void fill_lit(void *p);
void fill_const(void *p);
void fill_sibling(void *p, uint64_t n);
void fill_two(struct two *t);
void fill_late(void *p);

#endif

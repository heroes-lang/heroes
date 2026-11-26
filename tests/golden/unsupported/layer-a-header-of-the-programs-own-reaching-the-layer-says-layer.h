/* A header of the program's own: it reaches the platform layer's clock two
   includes down, through the header beside it. */
#include "layer-a-header-of-the-programs-own-reaching-the-layer-says-layer-below.h"

static inline int64_t ticks_twice(void) { return 2 * hero_time_monotonic_ns(); }

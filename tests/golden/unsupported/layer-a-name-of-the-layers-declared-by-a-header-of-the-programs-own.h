#include <stdint.h>

/* The layer's clock, declared here by the program itself rather than through
   the layer's `hero_time.h`. */
int64_t hero_time_monotonic_ns(void);

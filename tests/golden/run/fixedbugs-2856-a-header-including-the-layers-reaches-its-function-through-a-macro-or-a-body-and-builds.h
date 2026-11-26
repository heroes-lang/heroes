/* The layer's clock reached through a macro and a body of the program's own
 * header, which includes the layer's `hero_time.h`: clang holds every call
 * here to the layer's own declaration, and the group says `layer`. */
#include "hero_time.h"
#define now_ns hero_time_monotonic_ns
static inline int64_t now_inline(void) { return hero_time_monotonic_ns(); }

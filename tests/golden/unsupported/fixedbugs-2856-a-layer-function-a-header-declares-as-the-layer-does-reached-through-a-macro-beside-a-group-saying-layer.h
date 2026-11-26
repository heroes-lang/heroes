/* The layer's clock declared as `hero_time.h` declares it, by the program's
 * own header, which includes none of the layer's, and a macro for it. */
#include <stdint.h>
int64_t hero_time_monotonic_ns(void);
#define now_ns hero_time_monotonic_ns

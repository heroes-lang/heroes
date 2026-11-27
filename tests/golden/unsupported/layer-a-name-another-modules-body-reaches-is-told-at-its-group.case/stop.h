#include <stdint.h>

int64_t hero_net_stop_asked(void);

static inline int64_t stop_now(void) { return hero_net_stop_asked(); }

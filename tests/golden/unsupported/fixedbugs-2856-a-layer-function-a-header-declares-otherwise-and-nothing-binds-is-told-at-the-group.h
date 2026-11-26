/* The layer's `hero_net_ask_stop` declared otherwise than `hero_net.h` does
 * (`void`), and nothing the program binds from here reaches it. */
#include <stdint.h>
int hero_net_ask_stop(void);
static inline int64_t answer(void) { return 42; }

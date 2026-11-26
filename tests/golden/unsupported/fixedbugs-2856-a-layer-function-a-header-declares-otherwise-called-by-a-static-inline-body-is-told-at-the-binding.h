/* The layer's `hero_net_ask_stop` declared otherwise than `hero_net.h` does
 * (`void`), and called by a body of the header's own. */
int hero_net_ask_stop(void);
static inline int stop_inline(void) { return hero_net_ask_stop(); }

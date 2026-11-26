/* A header of the program's own declaring the layer's `hero_net_ask_stop`
 * otherwise than `hero_net.h` does (`void`), and a macro for it: the
 * binding of `stop_now` is a call of the layer's function through this
 * declaration. */
int hero_net_ask_stop(void);
#define stop_now hero_net_ask_stop

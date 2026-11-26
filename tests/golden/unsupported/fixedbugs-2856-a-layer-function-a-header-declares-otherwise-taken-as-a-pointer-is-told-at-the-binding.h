/* The layer's `hero_net_ask_stop` declared otherwise than `hero_net.h` does
 * (`void`), its address taken by a body of the header's own and called. */
int hero_net_ask_stop(void);
static inline int stop_via(void) {
    int (*f)(void) = hero_net_ask_stop;
    return f();
}

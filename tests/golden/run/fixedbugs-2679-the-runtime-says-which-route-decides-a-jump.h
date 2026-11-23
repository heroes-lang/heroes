/* The accessor of panel 255's R1: which route decides a jump here. Declared
 * by hand, since no header of the runtime's names it: it is the test's door
 * and no program's. */
#ifndef FIXEDBUGS_2679_ROUTE_H
#define FIXEDBUGS_2679_ROUTE_H
#include <stdint.h>
int hero_signal_jump_route(void);
static inline int64_t route_now(void) { return hero_signal_jump_route(); }
#endif

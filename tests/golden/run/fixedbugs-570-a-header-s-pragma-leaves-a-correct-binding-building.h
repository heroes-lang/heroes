/* Defect 570, 2026-10-10: pragmas a correct binding builds through. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <stdlib.h>
struct a { int x; };
static inline int take_a(struct a *p) { return p == 0; }

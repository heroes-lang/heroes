#ifndef FIXEDBUGS_563_WIDTH
#define FIXEDBUGS_563_WIDTH 8
#endif
static inline int width(Wide w) { return w.v + FIXEDBUGS_563_WIDTH; }

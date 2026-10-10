#ifndef LIMIT
#define LIMIT 10
#endif
static inline int cap(int v) { return v > LIMIT ? LIMIT : v; }

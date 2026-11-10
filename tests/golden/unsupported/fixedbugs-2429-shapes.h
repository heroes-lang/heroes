/* Declares what the group naming it binds, and what the group naming
 * `fixedbugs-2429-empty.h` binds under that header (defect 2429). */
#ifndef FIXEDBUGS_2429_SHAPES_H
#define FIXEDBUGS_2429_SHAPES_H
#define SPOT_MAX 9
struct spot { long long x; };
static inline long long area(long long w, long long h) { return w * h; }
static inline long long shape_count(void) { return 3; }
#endif

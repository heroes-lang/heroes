/* Defect 2389, 2026-11-09: a record whose typedef is named as the C the
 * compiler writes names a function's first local `p`, `h0_p`. */
#include <stdint.h>
typedef struct { int64_t a; } h0_p;
static inline int64_t geth(h0_p v) { return v.a; }

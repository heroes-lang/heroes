/* Defect 2389, 2026-11-09: a record whose typedef is named as the C the
 * compiler writes names its first temporary, `t1`, built, received, lent and
 * held in another record. */
#include <stdint.h>
typedef struct { int64_t a; } t1;
typedef struct { t1 inner; int64_t b; } outer;
static inline int64_t get(t1 v) { return v.a; }
static inline t1 make(int64_t x) { t1 r; r.a = x; return r; }
static inline void bump(t1 *v) { v->a += 1; }
static inline int64_t sum(outer o) { return o.inner.a + o.b; }

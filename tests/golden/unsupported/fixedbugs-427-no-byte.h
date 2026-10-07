/* Beside defect 427's refused cases: pointers to `const` things that are not
   bytes, which a `cstr` may not cross to. */
#include <stdint.h>

struct hero_pair { int32_t a; int32_t b; };

static inline int32_t r_int(const int *p) { return p[0]; }
static inline int32_t r_pair(const struct hero_pair *p) { return p->a; }

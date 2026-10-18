/* Panel 217's R3, M-readonly-lend: a plain struct C reads through a pointer
   to const, with no header any platform lacks, so the case runs on all four. */
#include <stdint.h>

struct pt { int64_t x; int32_t y; };

static inline int64_t pt_sum(const struct pt *p) { return p->x + p->y; }
static inline int64_t pt_scaled(const struct pt *p, int64_t k) { return (p->x + p->y) * k; }

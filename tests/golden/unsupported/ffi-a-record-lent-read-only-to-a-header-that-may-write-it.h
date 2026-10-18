/* Panel 217's R3, M-readonly-lend: four functions taking a struct's address,
   one through a pointer to const and three through pointers C may write. */
#include <stdint.h>

struct pt { int64_t x; int32_t y; };

static inline int64_t pt_value(const struct pt *p) { return p->x; }
static inline int32_t pt_reset(struct pt *p) { p->x = 0; return 0; }
static inline int64_t pt_peek_any(const void *p) { return ((const struct pt *)p)->x; }
static inline int64_t pt_poke_any(void *p) { ((struct pt *)p)->x += 100; return 0; }

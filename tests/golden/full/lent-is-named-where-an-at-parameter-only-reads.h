/* Panel 217's R4, M-readonly-lend: a C function that only reads the struct
   it is handed, bound with `@` as a reader of panel 217's arms bound it. */
#include <stdint.h>

struct pt { int64_t x; };

static inline int64_t pt_value(const struct pt *p) { return p->x; }

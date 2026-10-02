/* Defect 168, lane cb4, 2026-10-02: `E` has a member `y` the record leaves
   out. A header of this same name in tests/golden/run/ holds `E` whole, so
   the layout check must ask each directory's own. */
#include <stdint.h>
typedef struct { int32_t x; int32_t y; } E;
static inline E hero_168_make(int32_t x) { E e = { x, 2 }; return e; }

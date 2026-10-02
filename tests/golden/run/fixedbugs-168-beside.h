/* Defect 168, lane cb4, 2026-10-02: `E` is the record's whole. A header of
   this same name in tests/golden/unsupported/ gives `E` a member the record
   leaves out, so the layout check must ask each directory's own. */
#include <stdint.h>
typedef struct { int32_t x; } E;
static inline E hero_168_make(int32_t x) { E e = { x }; return e; }

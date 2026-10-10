/* Defect 555, lane b18-ffi, 2026-10-10: a `static inline twice` of `int`,
   this unit's own. */
static inline int twice(int x) { return x + x; }

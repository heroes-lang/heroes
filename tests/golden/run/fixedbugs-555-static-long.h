/* Defect 555, lane b18-ffi, 2026-10-10: a `static inline twice` of
   `long long`, the other unit's own. */
static inline long long twice(long long x) { return 2 * x; }

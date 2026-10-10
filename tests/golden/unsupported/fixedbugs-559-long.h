/* Defect 559, lane b18-ffi, 2026-10-10: `twice` of `long long`, which a module
   binds and which no unit of the other module reads. */
static inline long long twice(long long x) { return 2 * x; }

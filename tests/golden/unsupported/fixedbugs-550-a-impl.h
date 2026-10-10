/* Defect 550, lane b18-ffi, 2026-10-10: `twice` of `long long`, reached
   through `fixedbugs-550-a.h`. */
static inline long long twice(long long x) { return 2 * x; }

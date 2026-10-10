/* Defect 555, lane b18-ffi, 2026-10-10: `helper` of `long long`, which no
   group binds and this header's own code calls. */
long long helper(long long x);
static inline long long quadruple(long long x) { return 4 * helper(x); }

/* Defect 555, lane b18-ffi, 2026-10-10: `helper` of `double`, the same
   symbol declared another way. */
double helper(double x);
static inline double half(double x) { return helper(x) / 2; }

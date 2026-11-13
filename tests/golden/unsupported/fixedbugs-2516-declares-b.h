/* Defect 2516, 2026-11-13: `twice` of `double`, the same name declared
 * another way, and never called by this header's own code. */
double twice(double x);
static inline long long declares_b_value(long long x) { return x + 2; }

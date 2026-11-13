/* Defect 2516, 2026-11-13: `twice` of `long long`, called by the body of
 * the function this header's group binds. */
long long twice(long long x);
static inline long long calls_a_value(long long x) { return twice(x) + 1; }

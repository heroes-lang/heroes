/* Defect 2516, 2026-11-13: `twice` of `long long`, declared and never
 * called by this header's own code. */
long long twice(long long x);
static inline long long declares_a_value(long long x) { return x + 1; }

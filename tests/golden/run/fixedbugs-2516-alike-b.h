/* Defect 2516, 2026-11-13: `twice` of `long long` again, the two
 * declarations alike. */
long long twice(long long x);
static inline long long alike_b_value(long long x) { return x + 2; }

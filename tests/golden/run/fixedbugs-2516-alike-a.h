/* Defect 2516, 2026-11-13: `twice` of `long long`. */
long long twice(long long x);
static inline long long alike_a_value(long long x) { return x + 1; }

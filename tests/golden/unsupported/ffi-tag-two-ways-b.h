/* Found beside defect 2508, 2026-11-13: `twice` declared another way, called
 * by this header's own code. */
long long twice(long long x);
static inline long long tag_b_value(long long x) { return twice(x) + 2; }

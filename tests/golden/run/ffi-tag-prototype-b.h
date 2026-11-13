/* Found beside defect 2508, 2026-11-13: `twice` with its prototype, the same
 * result. */
struct 漢 { long long v; };
struct 漢 twice(long long x);
static inline long long proto_b_value(long long x) { return x + 2; }

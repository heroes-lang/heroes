/* Found beside defect 2508, 2026-11-13: `twice` returning a struct whose tag
 * is above ASCII, called by this header's own code. */
struct 漢 { long long v; };
struct 漢 twice(long long x);
static inline long long tag_a_value(long long x) { return twice(x).v; }

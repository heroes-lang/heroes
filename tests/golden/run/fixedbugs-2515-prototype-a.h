/* Defect 2515, 2026-11-13: `twice` with no prototype, returning
 * a struct whose tag is above ASCII. */
struct 漢 { long long v; };
struct 漢 twice();
static inline long long proto_a_value(long long x) { return x + 1; }

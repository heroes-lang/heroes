/* Panel 214, 2026-10-14: the culprit, poisoning C's own `int64_t`. */
#pragma GCC poison int64_t
static inline int three(void) { return 3; }

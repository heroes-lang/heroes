/* Defect 2044, 2026-10-13: a header that misspells `int16_t`, which clang
 * offers in its place, noting where C declares it. */
static inline int46_t seven(void) { return 7; }

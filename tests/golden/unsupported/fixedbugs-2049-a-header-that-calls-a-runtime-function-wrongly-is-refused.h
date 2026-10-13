/* Defect 2049, 2026-10-13: the third reproducer, a header that calls
 * `hero_str_len`, a function the Heroes runtime declares in every unit,
 * with an int. */
static inline int eight(void) { return hero_str_len(8); }

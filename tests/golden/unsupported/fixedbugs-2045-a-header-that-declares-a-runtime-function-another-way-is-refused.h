/* Defect 2045, 2026-10-13: a header that declares `hero_str_len`, a function
 * the Heroes runtime declares in every unit, with another signature. */
int hero_str_len(int x);
static inline int seven(void) { return 7; }

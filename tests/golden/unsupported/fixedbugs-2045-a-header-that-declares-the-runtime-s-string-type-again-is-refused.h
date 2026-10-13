/* Defect 2045, 2026-10-13: a header that declares `HeroStr`, the type the
 * Heroes runtime declares in every unit, as an object. */
extern int HeroStr;
static inline int six(void) { return 6; }

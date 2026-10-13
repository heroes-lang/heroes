/* Defect 2049, 2026-10-13: the fourth reproducer, a header that writes
 * `HeroSt`, which clang corrects to the Heroes runtime's `HeroStr`. */
static inline int eight(void) { HeroSt s; (void)s; return 8; }

/* Defect 2049, 2026-10-13: a header that writes `HeroFailur`, which
 * clang corrects to the Heroes runtime's `HeroFailure`. */
static inline int eight(void) { HeroFailur f; (void)f; return 8; }

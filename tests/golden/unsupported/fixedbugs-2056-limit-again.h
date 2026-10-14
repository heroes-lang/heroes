/* Defect 2056, 2026-10-14: a header that defines another group's constant
 * again, its `#undef` first, so clang says nothing of the redefinition. */
#undef LIMIT
#define LIMIT 50
static inline int bee(void) { return 2; }

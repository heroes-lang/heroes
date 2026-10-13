/* Defect 2051, 2026-10-13: the reproducer, a header that declares `fabs`,
 * a function of the C library clang declares itself, as a variable. */
int fabs;
static inline int eight(void) { return 8; }

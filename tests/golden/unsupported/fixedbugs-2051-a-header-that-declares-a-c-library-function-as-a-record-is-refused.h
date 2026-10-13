/* Defect 2051, 2026-10-13: a header that declares `malloc`, a function of
 * the C library clang declares itself, as a variable of a record. */
struct counted { int n; } malloc;
static inline int eight(void) { return 8; }

/* Defect 2052, 2026-10-13: a header that calls `undeclared_eight`, which
 * no file of the unit declares and no library defines. */
static inline int eight(void) { return undeclared_eight(8); }

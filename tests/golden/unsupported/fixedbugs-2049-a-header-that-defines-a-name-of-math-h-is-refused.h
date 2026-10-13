/* Defect 2049, 2026-10-13: the first reproducer, a header that defines
 * `sqrt`, a name of C's own <math.h>, which every unit of the program
 * reads after its groups' headers. */
static inline int sqrt(int x) { return x; }
static inline int eight(void) { return 8; }

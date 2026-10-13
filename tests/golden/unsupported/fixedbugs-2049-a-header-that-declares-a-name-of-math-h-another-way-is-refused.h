/* Defect 2049, 2026-10-13: a header that declares `floor`, a name of C's
 * own <math.h>, as a function of another type. */
double floor(int x);
static inline int eight(void) { return 8; }

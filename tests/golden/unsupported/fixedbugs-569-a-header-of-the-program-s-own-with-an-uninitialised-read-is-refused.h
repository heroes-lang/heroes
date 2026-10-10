/* Defect 569, 2026-10-10: a header whose own function reads an uninitialised value. */
#include <stdint.h>
static inline int unin(int x) { int y; if (x) y = 1; return y; }

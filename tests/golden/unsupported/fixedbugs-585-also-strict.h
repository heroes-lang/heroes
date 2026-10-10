/* Defect 585, 2026-10-10: a second header of the program's own that narrows the C library. */
#define _POSIX_C_SOURCE 200112L
static inline long long also_strict_on(void) { return 1; }

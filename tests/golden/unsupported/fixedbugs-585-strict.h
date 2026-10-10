/* Defect 585, 2026-10-10: a header of the program's own that narrows the C library to POSIX 2001. */
#define _POSIX_C_SOURCE 200112L
static inline long long strict_on(void) { return 1; }

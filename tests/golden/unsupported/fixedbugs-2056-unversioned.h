/* Defect 2056, 2026-10-14: a header that undefines another group's macro. */
#undef versioned
static inline int nine(void) { return 9; }

/* Defect 2056, 2026-10-14: a header that makes `foo`, a name another
 * group's header defines, a macro of its own `bar`. */
static inline int bar(void) { return 2; }
#define foo bar
static inline int eight(void) { return 8; }

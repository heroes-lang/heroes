/* Defect 2056, 2026-10-14: a header whose `versioned` is a macro naming its
 * second version, as a library renames a function it replaced. */
static inline int versioned(void) { return 1; }
static inline int versioned_two(void) { return 2; }
#define versioned versioned_two

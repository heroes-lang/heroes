/* Defect 2052, 2026-10-13: the reproducer, a header that calls
 * `hero_str_le`, a function nobody declared, which clang corrects to the
 * Heroes runtime's `hero_str_len`. */
static inline int eight(void) { return (int)hero_str_le(8); }

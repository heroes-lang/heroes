/* Defect 2049, 2026-10-13: a header that calls `hero_str_len` with an int
 * inside a call of another function of the runtime's. */
static inline int eight(void) { hero_print_int(hero_str_len(8)); return 8; }

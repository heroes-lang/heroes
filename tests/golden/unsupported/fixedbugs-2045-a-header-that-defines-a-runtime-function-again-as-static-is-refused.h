/* Defect 2045, 2026-10-13: the reproducer, a header that defines
 * `hero_str_len`, a function the Heroes runtime declares in every unit. */
#include <stdint.h>
static inline int hero_str_len(int x) { return x; }
static inline int eight(void) { return hero_str_len(8); }

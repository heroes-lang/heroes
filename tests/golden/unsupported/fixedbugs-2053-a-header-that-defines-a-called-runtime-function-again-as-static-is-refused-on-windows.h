/* Defect 2053, 2026-10-13: the reproducer, a header that defines
 * `hero_str_len` again as static, with the Heroes runtime's own types, in a
 * program whose `len()` calls it. */
#include <stdint.h>
static inline int64_t hero_str_len(HeroStr s) { (void)s; return 42; }
static inline int eight(void) { return 8; }

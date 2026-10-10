/* Defect 592, 2026-10-11: a header declaring a function of the
   compiler's runtime again with an `__asm__` label. */
#include <stdlib.h>
void hero_print_int(int64_t v) __asm__("fixedbugs_592_nowhere_print");

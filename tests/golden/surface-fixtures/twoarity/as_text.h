/* `as_text.hero`'s door to the same `printf` (panel 243's R1, defect 2421),
   at another shape: a string, read and kept by nothing. */
#include <stdio.h>

static inline int say_text(const char *s) {
    return printf("s = %s\n", s);
}

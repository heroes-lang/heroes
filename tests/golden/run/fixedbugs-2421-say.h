/* Defect 2421's route for a format function: a header of the program's own
   whose functions call `printf` with the format written inside as a
   literal, each value at a parameter of the C type its conversion names, so
   clang's `-Wformat` reads each conversion against its argument. */
#include <stdio.h>

static inline int say_count(long long n) {
    return printf("n = %lld\n", n);
}

static inline int say_text(const char *s) {
    return printf("s = %s\n", s);
}

/* The door this case calls `printf` through (panel 243's R1, defect 2421):
   a function of a new name with a fixed prototype, the format written inside
   as a literal, which clang's `-Wformat` reads against `n`. `long long` is
   the C type `%lld` names on every platform. */
#include <stdio.h>

static inline int print_count(long long n) {
    return printf("%lld\n", n);
}

/* `as_int.hero`'s door to `printf` (panel 243's R1, defect 2421): a function
   of a new name with a fixed prototype, the format inside as a literal that
   clang reads against `n`. */
#include <stdio.h>

static inline int say_count(long long n) {
    return printf("n = %lld\n", n);
}

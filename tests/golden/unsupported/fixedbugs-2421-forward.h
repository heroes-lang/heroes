/* Defect 2421's door, written wrong: a function of the program's own header
   with a fixed prototype that hands `printf` the format it was given, so no
   literal is there for clang to read against `v`. */
#include <stdio.h>

static inline int say_any(const char *f, long long v) {
    return printf(f, v);
}

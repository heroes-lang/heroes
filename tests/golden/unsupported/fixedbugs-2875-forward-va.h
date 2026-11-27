/* Issue 2875's shape beside defect 2421's door: a function of the program's
   own header that hands `vprintf` the format and the `va_list` it was given,
   so no literal is there for clang to read against the arguments. */
#include <stdarg.h>
#include <stdio.h>

static inline int say_list(const char *f, va_list ap) {
    return vprintf(f, ap);
}

static inline int say_count(void) {
    return 3;
}

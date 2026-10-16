/* Defect 2102: a variadic function that writes the string a call hands it
   past its `...`, which it reads as `char *`. */
#include <stdarg.h>
#include <stdint.h>

static inline int64_t va_scribble(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    char *s = va_arg(ap, char *);
    s[0] = 'X';
    va_end(ap);
    return k;
}

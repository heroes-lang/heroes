/* Defect 2102's legal shape: a variadic function that writes the string a
   call hands it past its `...`, and one that only reads it. */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

static inline int64_t va_scribble(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    char *s = va_arg(ap, char *);
    s[0] = 'X';
    va_end(ap);
    return k;
}

static inline int64_t va_length(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    const char *s = va_arg(ap, const char *);
    int64_t n = (int64_t)strlen(s);
    va_end(ap);
    return k * 100 + n;
}

/* Defect 2465, panel 245's R2, 2026-11-11: a variadic of the program's own
 * header, which every platform's C declares alike. */
#include <stdint.h>
#include <stdarg.h>
static inline int64_t pick(int64_t n, ...) { va_list a; va_start(a, n); int64_t got = n > 0 ? va_arg(a, int64_t) : 0; va_end(a); return got; }

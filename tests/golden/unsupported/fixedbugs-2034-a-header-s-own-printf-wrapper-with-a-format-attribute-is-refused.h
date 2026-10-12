/* Defect 2034, 2026-10-12: a header's own vprintf wrapper, declared with
 * the format attribute, handed an argument its format does not match. */
#include <stdarg.h>
#include <stdio.h>
__attribute__((format(printf, 1, 2))) static inline int say(const char *f, ...) { va_list ap; va_start(ap, f); int r = vprintf(f, ap); va_end(ap); return r; }
static inline int fmt_shout(void) { return say("%d\n", "seven"); }

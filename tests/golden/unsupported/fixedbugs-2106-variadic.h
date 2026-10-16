/* Defect 2106: two variadic functions that write through the pointer a call
   hands them past their `...`, wider than the program's cell. */
#include <stdarg.h>
#include <stdint.h>

typedef struct {
    int32_t a;
    int32_t b;
} Pair;

static inline int64_t va_wide(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    int64_t *n = va_arg(ap, int64_t *);
    *n = -1;
    va_end(ap);
    return k;
}

static inline int64_t va_pair(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    Pair *p = va_arg(ap, Pair *);
    p->a = 7;
    p->b = 9;
    va_end(ap);
    return k;
}

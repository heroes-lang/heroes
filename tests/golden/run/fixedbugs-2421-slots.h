/* Defect 2421: variadic functions, a library's as far as the program is
   concerned, that read what a call hands them past their `...` as the type
   their `va_arg` names. */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int32_t a;
    int32_t b;
} Pair;

static inline long slot_read(int count, ...) {
    va_list ap;
    va_start(ap, count);
    long got = va_arg(ap, long);
    va_end(ap);
    return got;
}

static inline double slot_double(int count, ...) {
    va_list ap;
    va_start(ap, count);
    double got = va_arg(ap, double);
    va_end(ap);
    return got;
}

static inline int64_t slot_pair(int count, ...) {
    va_list ap;
    va_start(ap, count);
    Pair p = va_arg(ap, Pair);
    va_end(ap);
    return p.a * 10 + p.b;
}

static inline int64_t slot_length(int count, ...) {
    va_list ap;
    va_start(ap, count);
    const char *s = va_arg(ap, const char *);
    va_end(ap);
    return (int64_t)strlen(s);
}

static inline int64_t slot_pointer(int count, ...) {
    va_list ap;
    va_start(ap, count);
    void *p = va_arg(ap, void *);
    va_end(ap);
    return p != 0;
}

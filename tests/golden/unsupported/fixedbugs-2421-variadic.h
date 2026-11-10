/* Defect 2421: variadic functions that read what a call hands them past
   their `...` as the type their `va_arg` names, which no prototype states:
   a `long`, an `int`, a `double`, a string, a struct, a pointer, a function
   pointer, and an `int64_t *` written through. */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int32_t a;
    int32_t b;
} Pair;

typedef struct slot_handle slot_handle;

static inline long slot_read(int count, ...) {
    va_list ap;
    va_start(ap, count);
    long got = va_arg(ap, long);
    va_end(ap);
    return got;
}

static inline int slot_flag(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int got = va_arg(ap, int);
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

static inline double slot_float(int count, ...) {
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

static inline int64_t slot_pointer(int count, ...) {
    va_list ap;
    va_start(ap, count);
    void *p = va_arg(ap, void *);
    va_end(ap);
    return p != 0;
}

static inline int64_t slot_handle_of(int count, ...) {
    va_list ap;
    va_start(ap, count);
    slot_handle *h = va_arg(ap, slot_handle *);
    va_end(ap);
    return h != 0;
}

static inline int64_t slot_callback(int count, ...) {
    va_list ap;
    va_start(ap, count);
    void (*f)(void) = va_arg(ap, void (*)(void));
    va_end(ap);
    f();
    return count;
}

static inline int64_t slot_wide(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int64_t *n = va_arg(ap, int64_t *);
    *n = -1;
    va_end(ap);
    return count;
}

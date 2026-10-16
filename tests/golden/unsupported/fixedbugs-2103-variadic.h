/* Defect 2103: two variadic functions that write four bytes through the
   pointer a call hands them past their `...`. */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t nsap[8];
    int64_t id;
} Slot;

static inline int64_t va_scrub(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    unsigned char *p = va_arg(ap, unsigned char *);
    memset(p, 0, 4);
    va_end(ap);
    return k;
}

static inline int64_t va_fill(int32_t k, ...) {
    va_list ap;
    va_start(ap, k);
    unsigned char *md = va_arg(ap, unsigned char *);
    memset(md, 7, 4);
    va_end(ap);
    return k;
}

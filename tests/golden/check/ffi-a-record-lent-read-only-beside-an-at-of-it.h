/* Panel 217's R3(d), M-readonly-lend: a writable and a read-only pointer
   C's restrict promises apart. Handed one address twice, the
   ffi-pragmatist measured 2 2 2 at -O0 and 1 2 1 at -O2, on this Mac and in
   Linux arm64 alike. */
#include <stdint.h>

struct pt { int64_t x; };

static inline int64_t mv(struct pt *restrict dst, const struct pt *restrict src) {
    dst->x += 1;
    return src->x;
}

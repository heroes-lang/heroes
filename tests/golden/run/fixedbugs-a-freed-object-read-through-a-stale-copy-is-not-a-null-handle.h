/* A C object with a pointer inside it, given back the way OpenSSL gives back —
 * cleansed to zero — and read again through a copy of its handle the program
 * kept. The read lands on the null field of a cleansed object, so the fault is
 * at a small offset, exactly where a null handle's would be.
 *
 * A POOL, NOT MALLOC, and the reason is measured: the same case over `calloc`
 * and `free` printed a heap address as the value at exit 0 on Linux x86-64,
 * Linux arm64 and Windows, three of three each, because the freed block was
 * handed out again before the stale read and its field held whatever the new
 * owner wrote; only Darwin's allocator left it zero long enough to fault. A
 * pool keeps the cleansed object where it is, so the null is read on every
 * platform, and under `--sanitize` the fault is ASan's to report. */
#include <string.h>
typedef struct inner { long long x; } inner;
typedef struct outer { long long pad[66]; inner *in; } outer;
static inline __attribute__((noinline)) outer *make(void) {
    static inner leaf;
    static outer slot;
    leaf.x = 42;
    slot.in = &leaf;
    return &slot;
}
static inline __attribute__((noinline)) void release(outer *o) { memset(o, 0, sizeof *o); }
static inline __attribute__((noinline)) long long value(outer *o) { return o->in->x; }

/* The same cell behind a typedef, the shape panel 176's critic attacked:
 * `cstring_t *` is `const char **` once clang has read through the name, and
 * the compiler compares what clang prints, which is the canonical spelling. */
#include <stdlib.h>
#include <stdint.h>
typedef const char *cstring_t;
static inline __attribute__((noinline)) void fill_t(cstring_t *out) { *out = "lent"; }
static inline __attribute__((noinline)) void free_out(const char *p) { free((void *)(uintptr_t)p); }

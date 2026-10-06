/* A C function whose count is in ELEMENTS, as getgroups(2), poll(2), and
   every `T *p, size_t n` array API: it writes n int32_t values. */
#include <stdint.h>
struct box { int32_t v[4]; int64_t after; };
static inline int64_t fill_ints(void *p, uint64_t n) { int32_t *q = p; for (uint64_t i = 0; i < n; i++) q[i] = 0x41414141; return (int64_t)n; }

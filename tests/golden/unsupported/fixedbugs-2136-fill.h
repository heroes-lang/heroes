/* Defect 2136's header: a function that writes through a pointer to a
   64-bit integer. */
#include <stdint.h>
static inline void fill(int64_t *out) { *out = 7; }

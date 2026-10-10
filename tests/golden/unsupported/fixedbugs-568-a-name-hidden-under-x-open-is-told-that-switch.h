/* Defect 568, 2026-10-10: a function declared only under X/Open, as glibc's wchar.h does wcwidth. */
#include <stdint.h>
#ifdef _XOPEN_SOURCE
static inline int32_t hidden_width(int32_t c) { return c > 0; }
#endif

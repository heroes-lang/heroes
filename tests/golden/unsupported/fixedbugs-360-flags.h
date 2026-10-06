/* Defect 360, lane cli12, 2026-10-05: a header clang accepts under its own
   defaults and refuses under the flags every unit is compiled with, a 64-bit
   value narrowed to `int` in silence (`-Werror=shorten-64-to-32`). */
#include <stdint.h>
static inline int narrowed(long long x) { return x; }
static inline int64_t thrice(int64_t x) { return 3 * x; }

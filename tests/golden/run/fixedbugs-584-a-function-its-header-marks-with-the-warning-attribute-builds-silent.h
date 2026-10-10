/* Defect 584, 2026-10-10: GCC's `warning` attribute, which clang says at -O0. */
#include <stdint.h>
__attribute__((warning("careful"))) static inline int64_t warned(int64_t x) { return x; }

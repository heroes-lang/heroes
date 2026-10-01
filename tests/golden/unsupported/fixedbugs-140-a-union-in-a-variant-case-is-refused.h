/* Beside its case, lane emit, 2026-10-01: a union of two integers, so a map
   keyed by a value holding one meets the union rule and not the float one. */
#include <stdint.h>
typedef union { int32_t i; uint32_t n; } W;

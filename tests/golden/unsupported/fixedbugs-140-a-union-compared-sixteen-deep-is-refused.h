/* Beside its case, defect 140, lane emit, 2026-10-01: a union, which a
   record binds for reading and no program may compare. */
#include <stdint.h>
typedef union { int32_t i; float f; } U;

/* Beside its case, lane emit, 2026-10-01: a union of two integers, so a map
   keyed by a value holding one meets the union rule and not the float one. */
/* 2026-10-02, lane land186 (panel 186 R3): `b`, an arm narrower than the
   union, added for the case's new record. */
#include <stdint.h>
typedef union { int32_t i; uint32_t n; int8_t b; } W;

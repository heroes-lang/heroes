/* Defect 582, 2026-10-10: a header that packs its own record and never pops. */
#pragma pack(push, 1)
#include <stdint.h>
typedef struct { uint8_t a; int64_t b; } Packed582;
static inline Packed582 packed_make(int64_t b) { Packed582 p; p.a = 7; p.b = b; return p; }
static inline int64_t packed_b(Packed582 p) { return p.b; }
static inline int64_t packed_size(void) { return (int64_t)sizeof(Packed582); }

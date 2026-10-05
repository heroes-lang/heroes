/* Defect 337, lane b11-misc, 2026-10-04: a header whose `#line` names a file
   holding the byte 0xE9, written as the escape C reads, so this file is
   ASCII. clang prints the location of every diagnostic after it with that
   byte as it is: measured on Apple clang 21, `caf`, the byte, `.h:1:2:`.
   Corrected 2026-10-05, lane cli12 (defect 337's Windows row): clang 23.1.1
   refuses that escape, *invalid escape sequence '\351' in an unevaluated
   string literal*, so the byte stands raw in the `#line` below and this file
   is not ASCII. Measured the same day, clang takes the raw byte with a
   warning, `-Winvalid-source-encoding`, and prints the location with the
   byte as it is: Apple clang 21 on this Mac, Debian clang 18.1.8, 20.1.8 and
   22.1.8 on Linux arm64, and clang 23.1.1 on the Windows box. */
#include <stdint.h>
#line 1 "café.h"
#warning "a warning the header carries"

static inline int64_t wide_count(void) { return 5000000000LL; }

typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline float sa_f(SA s) { return s.f; }

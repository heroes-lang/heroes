/* Defect 337, lane b11-misc, 2026-10-04: a header whose `#line` names a file
   holding the byte 0xE9, written as the escape C reads, so this file is
   ASCII. clang prints the location of every diagnostic after it with that
   byte as it is: measured on Apple clang 21, `caf`, the byte, `.h:1:2:`. */
#include <stdint.h>
#line 1 "caf\351.h"
#warning "a warning the header carries"

static inline int64_t wide_count(void) { return 5000000000LL; }

typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline float sa_f(SA s) { return s.f; }

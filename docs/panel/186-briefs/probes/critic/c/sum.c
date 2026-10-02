#include <stdint.h>
#include "u.h"
typedef struct { int64_t k; union { int8_t a; int8_t b; }; } PADU;
_Static_assert(sizeof(SA) >= sizeof(int32_t)*4, "sum SA");
_Static_assert(sizeof(PADU) >= sizeof(int64_t) + 1 + 1, "sum PADU");

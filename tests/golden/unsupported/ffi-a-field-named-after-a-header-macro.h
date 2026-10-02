/* Beside its case, lane land186, 2026-10-02: a header's macros that are no
   members of the struct beside them, one a number and one an identifier. */
#include <stdint.h>
#define size 4
#define spare __spare
typedef struct { int32_t len; int32_t cap; } BUF;
typedef struct { int32_t len; int32_t cap; } BUF2;

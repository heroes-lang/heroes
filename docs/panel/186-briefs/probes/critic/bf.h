#include <stdint.h>
typedef struct { int32_t kind; uint32_t flag : 1; uint32_t rest : 31; } BF;
typedef struct { int32_t kind; union { int32_t i; float f; }; union { int16_t s; uint8_t b; }; } S2U;
typedef struct { int32_t kind; struct { union { int32_t i; float f; }; int32_t y; }; int32_t x; } DEEP;
static inline BF make_bf(void) { BF b = {1, 1, 5}; return b; }

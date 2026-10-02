#include <stdint.h>
typedef union { int32_t i; uint32_t n; } W;
static inline W make_w(void) { W w; w.i = 7; return w; }

#include <stdint.h>
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
static inline SA make_sa(void) { SA s; s.kind = 1; s.i = 12; s.x = 3; return s; }
static inline float sa_f(SA s) { return s.f; }
static inline int32_t sa_x(SA s) { return s.x; }

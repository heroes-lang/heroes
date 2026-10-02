#include <stdint.h>
union utag { int32_t i; float f; };
typedef union TU { int32_t i; float f; } TU;
static inline union utag make_u(void) { union utag u; u.i = 3; return u; }

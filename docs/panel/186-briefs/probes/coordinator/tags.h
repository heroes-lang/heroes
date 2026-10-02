#include <stdint.h>
union utag { int32_t i; float f; };
typedef union TU { int32_t i; float f; } TU;
static inline union utag make_u(void) { union utag u; u.i = 3; return u; }
static inline TU make_tu(void) { TU u; u.i = 4; return u; }
static inline int32_t read_u(union utag *p) { return p->i; }
static inline union utag *new_u(void) { static union utag u; u.i = 5; return &u; }

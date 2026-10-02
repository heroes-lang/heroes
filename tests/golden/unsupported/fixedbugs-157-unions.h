/* Beside defect 157's case, lane land186, 2026-10-02: the coordinator's
   `tu.h` from panel 186's probes, a union named only by its tag beside one
   a typedef names. */
#include <stdint.h>
union utag { int32_t i; float f; };
typedef union TU { int32_t i; float f; } TU;
static inline union utag make_u(void) { union utag u; u.i = 3; return u; }

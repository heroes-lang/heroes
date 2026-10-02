/* Beside defect 157's case, lane land186, 2026-10-02: the coordinator's
   `tu.h` from panel 186's probes, a union named only by its tag beside one
   a typedef names. */
#include <stdint.h>
union utag { int32_t i; float f; };
typedef union TU { int32_t i; float f; } TU;
static inline union utag make_u(void) { union utag u; u.i = 3; return u; }

/* Defect 157 widened, lane land186, 2026-10-02: what a handle over the tag
   reaches, and an enum's tag beside the union's. */
static inline union utag *new_u(void) { static union utag u; u.i = 5; return &u; }
static inline int32_t read_u(union utag *p) { return p->i; }
enum colour { RED, GREEN };
static inline enum colour *one_colour(void) { static enum colour c = GREEN; return &c; }

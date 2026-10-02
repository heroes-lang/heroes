/* Beside defect 157's run cases, lane land186, 2026-10-02: a union named
   only by its tag, and the typedef the program's own header gives it, the
   route `ffi_tag_is_a_union`'s note names. */
#include <stdint.h>
union utag { int32_t i; float f; };
static inline union utag make_u(void) { union utag u; u.i = 3; return u; }
static inline int32_t read_u(union utag *p) { return p->i; }
static inline union utag *one_u(void) { static union utag u; u.i = 5; return &u; }
typedef union utag UI;

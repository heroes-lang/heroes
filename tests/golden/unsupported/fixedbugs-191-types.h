/* Defect 191, lane cb4, 2026-10-02: two typedefs whose type has no layout,
   each bound by a record with fields. clang 18 and clang 21 word their
   refusal of a member read through the first differently. */
#include <stdint.h>

/* a typedef of a struct the header keeps opaque, as sqlite3.h does */
typedef struct opaque_s191 opaque191_t;

/* a typedef of void, as curl.h writes typedef void CURL; */
typedef void void191_t;

int32_t hero_191_take(opaque191_t *p);

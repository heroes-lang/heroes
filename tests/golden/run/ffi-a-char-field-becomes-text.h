/* Beside the case, so no leg skips it for a missing package, and because the
   point is the TWO SIGNS on one header: a plain `char` field and an
   `unsigned char` field, which a Heroes program binds as `i8[8]` and `u8[8]`
   and reads identically (panel 162, M-readable-bytes). */
#include <stdint.h>

typedef struct {
    char name[8];
    unsigned char raw[8];
    int32_t id;
} Tag;

static inline int32_t tag_id(Tag t) { return t.id; }

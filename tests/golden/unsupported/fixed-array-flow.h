/* A struct with a fixed-array member, beside the case that binds it.
   Local rather than a system library on purpose: this case must never be
   skipped for a missing package, because the rows it makes fire were exit 2 and
   exit 134 before panel 081 R3 and a skip is a pass. */
#include <stdint.h>

typedef struct {
    float x;
    float y;
    int32_t reserved[4];
} Lens;

static inline int32_t lens_first(Lens l) { return l.reserved[0]; }

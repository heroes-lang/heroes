/* A struct whose first member is a pointer to a type this header DECLARES and
 * never sizes — the exact shape the handle form types — sitting inside a struct
 * the completeness probe checks field by field.
 *
 * The header is shipped beside the case rather than borrowed from the system,
 * because the defect was first seen against `raylib.h` on one Mac and the case
 * has to fire on all three platforms. */
#include <stdint.h>

typedef struct Thing Thing;

typedef struct {
    Thing  *handle;
    double  ratio;
    int64_t count;
} Slot;

/* `static inline` and not `static`: this header reaches a second translation
 * unit that calls neither, and C11 exempts an unused inline where it would warn
 * on an unused static. The `warnings` suite holds this directory to zero. */
static inline Slot slot_make(int64_t n) {
    Slot s;
    s.handle = 0;
    s.ratio = 0.5;
    s.count = n;
    return s;
}

static inline int64_t slot_count(Slot s) { return s.count; }

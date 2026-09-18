/* Beside the case. The struct is the shape a C function FILLS: the caller owns
   it, the callee writes into it. `slot` is a tag so the Heroes `record` may be
   `partial` and name only the field this program reads (panel 163). */
#include <stdint.h>
#include <string.h>

struct slot {
    char name[8];
    int32_t id;
};

static inline int32_t slot_fill(struct slot *s) {
    memcpy(s->name, "filled", 7);
    s->id = 42;
    return 0;
}

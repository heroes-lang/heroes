/* `char[N]`, the shape a third of real fixed-array struct fields have, beside
   the case that binds it. Local rather than `<sys/utsname.h>` for two reasons:
   the Windows leg has no `utsname`, and a real POSIX one would need a
   256-element literal to construct — the separate gap this case does not claim
   to close. */
#include <stdint.h>

typedef struct {
    char name[4];
    int32_t id;
} Tag;

static inline int32_t tag_first(Tag t) { return (int32_t)t.name[0]; }

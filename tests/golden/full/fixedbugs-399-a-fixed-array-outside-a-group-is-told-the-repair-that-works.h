/* A C struct with a `char[4]` field, the shape of `struct utsname`'s five
   `char[256]` fields, beside the case that binds it. Local rather than
   `<sys/utsname.h>`: the Windows leg has no `utsname`. */
#include <stdint.h>

typedef struct {
    char bytes[4];
    int32_t id;
} Name;

static inline int32_t name_first(Name n) { return (int32_t)n.bytes[0]; }

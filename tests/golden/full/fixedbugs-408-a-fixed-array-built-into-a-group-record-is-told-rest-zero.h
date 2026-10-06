/* Three C structs with `char[N]` fields, beside the case that binds them:
   one built field by field, one with three fields of one length, the shape
   of `struct utsname`, and one with a field called `rest`, which the words
   `rest: zero` would hide. Local rather than `<sys/utsname.h>`: the Windows
   leg has no `utsname`. */
#include <stdint.h>

typedef struct {
    char name[4];
    int32_t id;
} Tag;

typedef struct {
    char sysname[8];
    char nodename[8];
    char machine[8];
} Uts;

typedef struct {
    char head[4];
    int32_t rest;
} Odd;

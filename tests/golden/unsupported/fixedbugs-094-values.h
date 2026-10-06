/* Defect 094, lane ffi13, 2026-10-06: values the unsupported cases of
   `fixedbugs-094-*` bind at a record they cannot build as written, each on a
   header of the program's own so every platform tells them. */
#include <stdint.h>

struct pt { int32_t x; int32_t y; };
struct other { int32_t x; int32_t y; };
struct cp { char c; int32_t y; };

/* A negative enumerator makes each enum `int` on every platform. */
enum colour { RED = -1, GREEN };
enum shade { LIGHT = -1, DARK };
struct painted { enum colour c; int32_t y; };

#define PT_EXCESS {1, 2, 3}
#define PT_NO_Z { .z = 1 }
#define CP_WIDE {300, 2}
#define PT_HALF {1.5, 2}
#define PAINTED_SHADE {DARK, 2}
#define PT_OTHER ((struct other){1, 2})
#define PT_SCALAR 5

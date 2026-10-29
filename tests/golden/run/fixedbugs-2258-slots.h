/* Defect 2258 (panel 232's ffi-pragmatist, `slots.h`): four static slots, so
 * two handles can be distinct, given back by cleansing, so no allocator
 * decides any answer here. */
#include <string.h>
typedef struct inner { long long x; } inner;
typedef struct outer { long long pad[66]; inner *in; } outer;
static inner leaves[4];
static outer slots[4];
static inline __attribute__((noinline)) outer *make_at(long long i) {
    leaves[i].x = 40 + i;
    slots[i].in = &leaves[i];
    return &slots[i];
}
static inline __attribute__((noinline)) void release(outer *o) { memset(o, 0, sizeof *o); }

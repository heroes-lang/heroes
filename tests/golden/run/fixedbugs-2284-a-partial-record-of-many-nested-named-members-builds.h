/* Defect 2284 (2026-11-07): a struct holding 512 named int64_t leaves at
   eight depths, pairs of pairs, as panel 233 measured it (`n8`). Bound
   `partial`, every member at every depth is read from clang's record
   layout, and the lock screen asked one question per member until this
   defect: one per type now. */
#include <stdint.h>
typedef struct r0 { int64_t a; int64_t b; } r0;
typedef struct r1 { r0 left; r0 right; } r1;
typedef struct r2 { r1 left; r1 right; } r2;
typedef struct r3 { r2 left; r2 right; } r3;
typedef struct r4 { r3 left; r3 right; } r4;
typedef struct r5 { r4 left; r4 right; } r5;
typedef struct r6 { r5 left; r5 right; } r6;
typedef struct r7 { r6 left; r6 right; } r7;
typedef struct heroes_nest {
    int64_t first;
    int64_t second;
    r0 a0;
    r1 a1;
    r2 a2;
    r3 a3;
    r4 a4;
    r5 a5;
    r6 a6;
    r7 a7;
} heroes_nest;
static inline int64_t heroes_nest_first(heroes_nest h) { return h.first + h.a7.right.right.right.right.right.right.right.b; }

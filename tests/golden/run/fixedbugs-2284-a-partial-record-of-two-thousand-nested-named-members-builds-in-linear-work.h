/* Defect 2284's remainder (2026-11-07): a struct holding 2,048 named
   int64_t leaves at ten depths, pairs of pairs, the issue's family at
   panel 233's `n10`. Bound `partial`, its padding table holds a span for
   every leaf the layout dump names, which the emitter gathered by copying
   the spans so far at every leaf. */
#include <stdint.h>
typedef struct r0 { int64_t a; int64_t b; } r0;
typedef struct r1 { r0 left; r0 right; } r1;
typedef struct r2 { r1 left; r1 right; } r2;
typedef struct r3 { r2 left; r2 right; } r3;
typedef struct r4 { r3 left; r3 right; } r4;
typedef struct r5 { r4 left; r4 right; } r5;
typedef struct r6 { r5 left; r5 right; } r6;
typedef struct r7 { r6 left; r6 right; } r7;
typedef struct r8 { r7 left; r7 right; } r8;
typedef struct r9 { r8 left; r8 right; } r9;
typedef struct fixedbugs_2284_nest {
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
    r8 a8;
    r9 a9;
} fixedbugs_2284_nest;
static inline int64_t fixedbugs_2284_nest_first(fixedbugs_2284_nest h) { return h.first + h.a9.right.right.right.right.right.right.right.right.right.b; }

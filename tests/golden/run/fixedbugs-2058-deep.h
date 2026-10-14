/* Defect 2058, 2026-10-14: records held by value forty deep, the deepest
 * holding a union, and one record holding the deepest and an array of the
 * next, so a `rest: zero` past the depth meets every shape it writes. */
#include <stdint.h>
typedef struct { union { int32_t i; float f; }; int64_t v; } D0;
typedef struct { D0 inner; int32_t n; } D1;
typedef struct { D1 inner; int32_t n; } D2;
typedef struct { D2 inner; int32_t n; } D3;
typedef struct { D3 inner; int32_t n; } D4;
typedef struct { D4 inner; int32_t n; } D5;
typedef struct { D5 inner; int32_t n; } D6;
typedef struct { D6 inner; int32_t n; } D7;
typedef struct { D7 inner; int32_t n; } D8;
typedef struct { D8 inner; int32_t n; } D9;
typedef struct { D9 inner; int32_t n; } D10;
typedef struct { D10 inner; int32_t n; } D11;
typedef struct { D11 inner; int32_t n; } D12;
typedef struct { D12 inner; int32_t n; } D13;
typedef struct { D13 inner; int32_t n; } D14;
typedef struct { D14 inner; int32_t n; } D15;
typedef struct { D15 inner; int32_t n; } D16;
typedef struct { D16 inner; int32_t n; } D17;
typedef struct { D17 inner; int32_t n; } D18;
typedef struct { D18 inner; int32_t n; } D19;
typedef struct { D19 inner; int32_t n; } D20;
typedef struct { D20 inner; int32_t n; } D21;
typedef struct { D21 inner; int32_t n; } D22;
typedef struct { D22 inner; int32_t n; } D23;
typedef struct { D23 inner; int32_t n; } D24;
typedef struct { D24 inner; int32_t n; } D25;
typedef struct { D25 inner; int32_t n; } D26;
typedef struct { D26 inner; int32_t n; } D27;
typedef struct { D27 inner; int32_t n; } D28;
typedef struct { D28 inner; int32_t n; } D29;
typedef struct { D29 inner; int32_t n; } D30;
typedef struct { D30 inner; int32_t n; } D31;
typedef struct { D31 inner; int32_t n; } D32;
typedef struct { D32 inner; int32_t n; } D33;
typedef struct { D33 inner; int32_t n; } D34;
typedef struct { D34 inner; int32_t n; } D35;
typedef struct { D35 inner; int32_t n; } D36;
typedef struct { D36 inner; int32_t n; } D37;
typedef struct { D37 inner; int32_t n; } D38;
typedef struct { D38 inner; int32_t n; } D39;
typedef struct { D39 deep; D38 pair[2]; int32_t tag; } W;
static inline int64_t w_deepest(const W *w) { return w->deep.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.v + w->pair[1].inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.inner.i; }

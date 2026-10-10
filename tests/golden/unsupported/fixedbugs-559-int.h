/* Defect 559, lane b18-ffi, 2026-10-10: `twice` of `int`, which cannot share
   one unit with `fixedbugs-559-long.h`, and what the program binds beside
   it. */
typedef struct Pt { int v; } Pt;
static inline int twice(int x) { return x + x; }
static inline void fill_int(int *x) { *x = 5; }
static inline void fill_wide(long long *x) { *x = 5; }
static inline int ptv(Pt p) { return p.v; }

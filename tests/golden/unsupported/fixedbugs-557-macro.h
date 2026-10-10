/* Defect 557, lane b18-ffi, 2026-10-10: a macro of the name another header
   declares as a function (the critic's `macro`). */
#define twice(x) ((x) * 2)
static inline long long dbl(long long x) { return twice(x); }

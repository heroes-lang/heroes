/* Defect 550, lane b18-ffi, 2026-10-10: `twice` of `int`, which cannot share
   one unit with the other's. */
static inline int twice(int x) { return x + x; }
static inline int thrice(int x) { return 3 * x; }

/* Defect 538, lane b17-emit, 2026-10-09: `twice` of `int`, which compiles
   alone and not beside `fixedbugs-538-long.h`. */
static inline int twice(int x) { return x + x; }
static inline int thrice(int x) { return 3 * x; }

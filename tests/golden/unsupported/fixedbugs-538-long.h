/* Defect 538, lane b17-emit, 2026-10-09: `twice` of `long`, which compiles
   alone and not beside `fixedbugs-538-int.h`. */
static inline long twice(long x) { return 2 * x; }

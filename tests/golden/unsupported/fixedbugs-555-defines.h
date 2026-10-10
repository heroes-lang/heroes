/* Defect 555, lane b18-ffi, 2026-10-10: the one definition of `twice` and of
   `helper`, external, which one module's unit compiles. */
long long twice(long long x) { return 2 * x; }
long long helper(long long x) { return 3 * x; }
static inline int one(void) { return 1; }

/* Defect 556, lane b18-ffi, 2026-10-10: a function defined in a header,
   neither `static` nor `inline`, so every unit that includes it defines it. */
long long twice(long long x) { return 2 * x; }

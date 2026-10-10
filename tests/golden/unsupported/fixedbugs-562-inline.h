/* Defect 562, lane b18-ffi, 2026-10-10: a C99 inline definition, neither
   `static` nor `extern`, which provides no external definition of `twice`
   (the ffi-pragmatist's `c99`). */
inline long long twice(long long x) { return 2 * x; }

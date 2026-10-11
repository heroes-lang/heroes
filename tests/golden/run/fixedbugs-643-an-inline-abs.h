/* Defect 643, lane b21-win, 2026-10-11: a C99 inline definition of a name
   clang knows as a library builtin, neither `static` nor `extern`, so no
   external definition: the C library's `abs` is the one a call that is not
   inlined reaches, on every platform. */
inline int abs(int x) { return x < 0 ? -x : x; }

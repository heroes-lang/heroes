/* Defect 557, lane b18-ffi, 2026-10-10: `twice` a function, which compiles
   alone and is no declaration after the other header's macro. */
static inline int twice(int x) { return x + x; }

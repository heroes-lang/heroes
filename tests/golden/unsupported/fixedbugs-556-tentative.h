/* Defect 556, lane b18-ffi, 2026-10-10: a tentative definition, which a
   `static inline` function beside it reads (the ffi-pragmatist's `tent`). */
int counter;
static inline int bump(void) { return ++counter; }

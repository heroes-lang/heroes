/* Defect 559, lane b18-ffi, 2026-10-10: a struct one module binds and
   another module's unit spells, passing it, holding it and comparing it. */
typedef struct Pt { int v; int w; } Pt;
static inline Pt pt_make(int v) { Pt p; p.v = v; p.w = 2 * v; return p; }

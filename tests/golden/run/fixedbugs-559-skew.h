/* Defect 559, lane b18-ffi, 2026-10-10: a struct laid out by a macro this
   header does not define, so it is `{ int a; int b; }` read alone. */
#ifdef WIDE
typedef struct S { long long a; int b; } S;
#else
typedef struct S { int a; int b; } S;
#endif
static inline int sget(S s) { return s.a + s.b; }

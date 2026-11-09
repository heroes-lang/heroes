#ifdef FIXEDBUGS_2364_WIDE
typedef struct In { long long a; int b; } In;
#else
typedef struct In { int a; int b; } In;
#endif
typedef struct Outer { int c; In r; } Outer;
static inline Outer outer_make(int v) { Outer o = { 7, { v, 2 } }; return o; }

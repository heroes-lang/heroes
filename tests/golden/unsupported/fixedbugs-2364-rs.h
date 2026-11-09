#ifdef FIXEDBUGS_2364_WIDE
typedef struct Rs { long long a; int b; } Rs;
#else
typedef struct Rs { int a; int b; } Rs;
#endif
static inline Rs rs_make(int v) { Rs x = { v, 2 }; return x; }

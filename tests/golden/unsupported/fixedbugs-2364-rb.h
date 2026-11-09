#define FIXEDBUGS_2364_RB_WIDE 1
#ifdef FIXEDBUGS_2364_PA_WIDE
typedef struct Rb { long long a; } Rb;
#else
typedef struct Rb { int a; } Rb;
#endif
static inline Rb rb_make(int v) { Rb x = { v }; return x; }

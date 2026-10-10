#ifdef FIXEDBUGS_563_WIDE
typedef struct Sk { long long a; int b; } Sk;
#else
typedef struct Sk { int a; int b; } Sk;
#endif

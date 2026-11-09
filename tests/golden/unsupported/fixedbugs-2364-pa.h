#define FIXEDBUGS_2364_PA_WIDE 1
#ifdef FIXEDBUGS_2364_RB_WIDE
typedef struct Pa { long long w; } Pa;
#else
typedef struct Pa { int w; } Pa;
#endif
static inline Pa pa_make(int v) { Pa x = { v }; return x; }

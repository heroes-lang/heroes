#define FIXEDBUGS_2364_WIDE 1
typedef struct Pw { int w; } Pw;
static inline Pw pw_make(int v) { Pw x = { v }; return x; }

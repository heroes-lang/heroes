#define FIXEDBUGS_563_WIDE 1
typedef struct Wh { int w; } Wh;
static inline Wh wh_make(int v) { Wh x = { v }; return x; }

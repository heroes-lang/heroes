#define FIXEDBUGS_2364_WIDE 1
typedef struct Mm { int w; } Mm;
static inline Mm mm_make(int v) { Mm x = { v }; return x; }

#define FIXEDBUGS_2364_PACKED 1
typedef struct Pk { int w; } Pk;
static inline Pk pk_make(int v) { Pk x = { v }; return x; }

typedef struct Pl { int w; } Pl;
static inline Pl pl_make(int v) { Pl x = { v }; return x; }
#pragma pack(1)

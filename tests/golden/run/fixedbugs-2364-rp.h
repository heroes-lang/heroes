#ifdef FIXEDBUGS_2364_PACKED
#pragma pack(push, 1)
#endif
typedef struct Rp { char c; int b; } Rp;
#ifdef FIXEDBUGS_2364_PACKED
#pragma pack(pop)
#endif
static inline Rp rp_make(int v) { Rp x = { 1, v }; return x; }

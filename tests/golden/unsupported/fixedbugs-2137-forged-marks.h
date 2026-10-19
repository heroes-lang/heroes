/* Defect 2137's header (panel 225's critic, `forge2` and `forge3`): a
   narrowing function, a pack the guard resets after every header, then the
   text of both prototypes' marks, as lines and through `_Pragma`. */
static inline int narrow(int x) { return x; }
#pragma pack(push, 1)
#pragma hero_probe_cut on
#pragma heroes_probe cut
#define HDR_MARK _Pragma("hero_probe_cut on") _Pragma("heroes_probe cut")
HDR_MARK
#define K_SZ ((-2147483647 - 1) / ((int)sizeof(struct { char c; int i; }) - 6))

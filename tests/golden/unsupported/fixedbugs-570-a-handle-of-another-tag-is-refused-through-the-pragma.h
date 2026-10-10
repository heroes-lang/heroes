/* Defect 570, 2026-10-10: a handle of another tag behind an ignored pragma. */
#pragma clang diagnostic ignored "-Wincompatible-pointer-types"
struct a { int x; };
struct b { int y; };
static inline int take_a(struct a *p) { return p == 0; }

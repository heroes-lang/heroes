#include <raylib.h>
#include <string.h>
#include "d2_decls.h"
_Static_assert(sizeof(struct hero_rec_Vector2) == sizeof(Vector2), "layout");
_Static_assert(sizeof(struct hero_rec_Color) == sizeof(Color), "layout");
struct hero_rec_Vector2 hero_shim_GetSplinePointLinear(struct hero_rec_Vector2 a, struct hero_rec_Vector2 b, float t) {
    Vector2 ca, cb, r; struct hero_rec_Vector2 out;
    memcpy(&ca, &a, sizeof ca); memcpy(&cb, &b, sizeof cb);
    r = GetSplinePointLinear(ca, cb, t);
    memcpy(&out, &r, sizeof out);
    return out;
}
int32_t hero_shim_ColorToInt(struct hero_rec_Color c) { Color cc; memcpy(&cc, &c, sizeof cc); return ColorToInt(cc); }

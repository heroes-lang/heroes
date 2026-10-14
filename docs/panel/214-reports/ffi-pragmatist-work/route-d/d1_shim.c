#include <raylib.h>
#include "d1_decls.h"
Vector2 hero_shim_GetSplinePointLinear(Vector2 a, Vector2 b, float t) { return GetSplinePointLinear(a, b, t); }
int hero_shim_ColorToInt(Color c) { return ColorToInt(c); }

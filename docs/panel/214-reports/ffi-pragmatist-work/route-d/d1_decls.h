/* Route D, d1: the emitter's one declarations file, read by the program's
 * unit (which never reads raylib.h) and by the shim's (which does), so clang
 * holds the shim's definitions to the program's declarations. The records
 * are re-declared from the Heroes `record`s, under the header's own tags. */
typedef struct Vector2 { float x; float y; } Vector2;
typedef struct Color { unsigned char r; unsigned char g; unsigned char b; unsigned char a; } Color;
Vector2 hero_shim_GetSplinePointLinear(Vector2 a, Vector2 b, float t);
int hero_shim_ColorToInt(Color c);

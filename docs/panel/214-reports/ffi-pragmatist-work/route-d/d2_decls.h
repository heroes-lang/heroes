/* Route D, d2: the program's records under the emitter's own tags, so the
 * shim's unit can hold them beside raylib.h's; the shim copies each record
 * across, both ways, on every call. */
#include <stdint.h>
struct hero_rec_Vector2 { float x; float y; };
struct hero_rec_Color { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
struct hero_rec_Vector2 hero_shim_GetSplinePointLinear(struct hero_rec_Vector2 a, struct hero_rec_Vector2 b, float t);
int32_t hero_shim_ColorToInt(struct hero_rec_Color c);

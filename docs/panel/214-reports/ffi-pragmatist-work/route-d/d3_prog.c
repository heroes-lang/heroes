/* Route D, d3: the program's unit re-declares the record under the
 * header's tag from the Heroes record; here the emitter wrote `double` for
 * an `f32` field. No unit holds both declarations, so nothing compares them. */
#include <stdio.h>
typedef struct Vector2 { double x; double y; } Vector2;
Vector2 hero_shim_GetSplinePointLinear(Vector2 a, Vector2 b, float t);
int main(void) {
    Vector2 m = hero_shim_GetSplinePointLinear((Vector2){0, 0}, (Vector2){10, 20}, 0.5f);
    printf("%g %g\n", m.x, m.y);
    return 0;
}

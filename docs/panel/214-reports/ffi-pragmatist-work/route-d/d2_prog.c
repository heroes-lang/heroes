#include <stdio.h>
#include "d2_decls.h"
int main(void) {
    struct hero_rec_Vector2 m = hero_shim_GetSplinePointLinear((struct hero_rec_Vector2){0, 0}, (struct hero_rec_Vector2){10, 20}, 0.5f);
    printf("%g %g %d\n", m.x, m.y, hero_shim_ColorToInt((struct hero_rec_Color){255, 0, 0, 255}));
    return 0;
}

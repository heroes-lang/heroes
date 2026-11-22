/* Panel 255, the ffi-pragmatist's libpng binding shim: libpng reports an error
 * by `longjmp` to the buffer `png_jmpbuf` names inside its own heap struct, so
 * the `setjmp` is the binding's C, the shape every libpng binding has. The
 * struct, and the `jmp_buf` in it, are freed by `png_destroy_read_struct`. */
#include <png.h>
#include <stdint.h>
#include <stdio.h>
static inline int32_t pd_decode(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return -1;
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png != NULL ? png_create_info_struct(png) : NULL;
    if (png == NULL || info == NULL) {
        fclose(f);
        return -4;
    }
    volatile int32_t result = -2;
    if (setjmp(png_jmpbuf(png))) {
        result = -3;
    } else {
        png_init_io(png, f);
        png_read_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
        result = (int32_t)(png_get_image_width(png, info) * 1000 + png_get_image_height(png, info));
    }
    png_destroy_read_struct(&png, &info, NULL);
    fclose(f);
    return result;
}

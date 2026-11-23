/* Defect 2679's origin (panel 255's R5): libpng 1.6, whose error path jumps
 * to the `jmp_buf` of its own `png_struct` and whose struct is then freed,
 * with the next decode saving into the next struct. A PNG is written to an
 * anonymous file, decoded, a truncated copy of it decoded (the error path),
 * and the first decoded again. */
#ifndef FIXEDBUGS_2679_LIBPNG_H
#define FIXEDBUGS_2679_LIBPNG_H
#include <png.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static FILE *png_good = NULL;
static FILE *png_bad = NULL;

/* libpng's own error path with its message left out: the jump is still
 * libpng's, `png_longjmp` to the `jmp_buf` of the struct. */
static void png_quiet_error(png_structp png, png_const_charp message) {
    (void)message;
    png_longjmp(png, 1);
}
static void png_quiet_warning(png_structp png, png_const_charp message) {
    (void)png;
    (void)message;
}

/* Writes a 64 by 48 RGB image to `png_good` and the first half of its bytes
 * to `png_bad`; 0, or a negative number where C itself failed. */
static inline int64_t png_make(void) {
    png_good = tmpfile();
    png_bad = tmpfile();
    if (png_good == NULL || png_bad == NULL) return -2;
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_quiet_error, png_quiet_warning);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        return -1;
    }
    png_init_io(png, png_good);
    png_set_IHDR(png, info, 64, 48, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);
    png_byte row[64 * 3];
    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 64 * 3; x++) row[x] = (png_byte)(x * 7 + y * 13);
        png_write_row(png, row);
    }
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fflush(png_good);
    rewind(png_good);
    char bytes[1 << 16];
    size_t n = fread(bytes, 1, sizeof bytes, png_good);
    if (n == 0 || fwrite(bytes, 1, n / 2, png_bad) != n / 2) return -3;
    fflush(png_bad);
    return 0;
}

/* The pixels of the good image (3072), or of the truncated one: -1, libpng's
 * jump out of its own error path. */
static inline int64_t png_decode(int64_t which) {
    FILE *f = which == 0 ? png_good : png_bad;
    rewind(f);
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_quiet_error, png_quiet_warning);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        return -1;
    }
    png_init_io(png, f);
    png_read_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
    int64_t pixels = (int64_t)png_get_image_width(png, info) * (int64_t)png_get_image_height(png, info);
    png_destroy_read_struct(&png, &info, NULL);
    return pixels;
}
#endif

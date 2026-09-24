/* Panel 175 compiler-engineer: the shapes beside route A. noinline for r1.h's reason. */
#include <stdlib.h>
typedef struct hh hh;
typedef struct tex tex;
typedef struct gly gly;
struct font { tex *texture; gly *glyphs; };
static __attribute__((noinline)) struct font load_font(void) { struct font f; f.texture = (tex *)malloc(8); f.glyphs = (gly *)malloc(8); return f; }
static __attribute__((noinline)) void unload_font(struct font f) { free(f.texture); free(f.glyphs); }
static __attribute__((noinline)) void unload_texture(tex *t) { free(t); }
static __attribute__((noinline)) hh *h_open(void) { return (hh *)malloc(8); }
static __attribute__((noinline)) void h_open_out(hh **out) { *out = (hh *)malloc(8); }
static __attribute__((noinline)) void h_close(hh *x) { free(x); }
static __attribute__((noinline)) void h_close2(hh *x) { free(x); }
static __attribute__((noinline)) void h_drop(hh *x) { free(x); }
static __attribute__((noinline)) hh *h_peek(hh *x) { return x; }
static __attribute__((noinline)) hh *h_grow(hh *x) { return (hh *)realloc(x, 64); }
/* One address, handed out again: C's allocator is not the only thing that reuses one. */
static char hero_fixed_cell[8];
static __attribute__((noinline)) hh *f_open(void) { return (hh *)hero_fixed_cell; }
static __attribute__((noinline)) void f_open_out(hh **out) { *out = (hh *)hero_fixed_cell; }
static __attribute__((noinline)) void f_close(hh *x) { (void)x; }
static __attribute__((noinline)) void f_close2(hh *x) { (void)x; }
static __attribute__((noinline)) void f_drop(hh *x) { (void)x; }

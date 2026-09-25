/* Panel 175's compiler-engineer's `shapes.h`, cut to the two shapes this
 * case asks about: a struct carrying two handles, raylib's `Font` by value,
 * and a struct carrying a fixed array of them, jpeglib's quantisation
 * tables. Static pools, so nothing here depends on an allocator; `check`
 * never opens it. */
#include <stdint.h>
typedef struct tex { int64_t w; } tex;
struct font { tex *texture; tex *atlas; };
struct sheet { tex *page[4]; };
static inline __attribute__((noinline)) tex *tex_new(void) { static tex pool[16]; static int next = 0; pool[next].w = 8 + next; return &pool[next++]; }
static inline __attribute__((noinline)) void tex_free(tex *t) { (void)t; }
static inline __attribute__((noinline)) int64_t tex_width(tex *t) { return t->w; }
static inline __attribute__((noinline)) int64_t font_area(struct font f) { return f.texture->w * f.atlas->w; }
static inline __attribute__((noinline)) struct sheet sheet_new(void) { struct sheet s; for (int i = 0; i < 4; i++) s.page[i] = tex_new(); return s; }
static inline __attribute__((noinline)) int64_t sheet_total(struct sheet s) { return s.page[0]->w + s.page[1]->w + s.page[2]->w + s.page[3]->w; }

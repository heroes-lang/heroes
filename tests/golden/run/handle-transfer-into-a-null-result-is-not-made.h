/* `realloc`'s shape over a pool: `grow` hands the block into the block it
 * returns and frees the old one — unless it fails, when it returns NULL and
 * the old block is still the caller's. */
#include <stdint.h>
#include <stddef.h>
typedef struct mem { int64_t size; int64_t live; } mem;
static inline __attribute__((noinline)) mem *mem_new(int64_t size) { static mem pool[4]; static int next = 0; pool[next].size = size; pool[next].live = 1; return &pool[next++]; }
static inline __attribute__((noinline)) mem *grow(mem *old, int64_t size) { if (size > 64) return NULL; old->live = 0; return mem_new(size); }
static inline __attribute__((noinline)) void mem_free(mem *m) { m->live = 0; }
static inline __attribute__((noinline)) int64_t mem_size(mem *m) { return m->size; }

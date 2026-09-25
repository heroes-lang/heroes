/* One static cell handed out every time, the allocator panel 175's completeness
 * critic built so that C's reuse of an address does not depend on libmalloc:
 * `cell_close` frees nothing, and the next `cell_open` is the same address. */
#include <stdint.h>

typedef struct cell cell;

struct cell {
    int64_t v;
};

static cell the_cell;
static int64_t cells_made = 0;

static inline __attribute__((noinline)) cell *cell_open(void) {
    cells_made++;
    the_cell.v = cells_made;
    return &the_cell;
}

static inline __attribute__((noinline)) void cell_close(cell *c) { c->v = -1; }

static inline __attribute__((noinline)) int64_t cell_value(cell *c) { return c->v; }

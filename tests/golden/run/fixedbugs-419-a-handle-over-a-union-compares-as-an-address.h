/* A union this header names and a typedef that lets a handle reach it, for
 * defect 419's case: a handle over a union is legal (the note of
 * `ffi_tag_is_a_union` says to write the typedef's name after `tag`), and
 * comparing two of them compares two addresses, never a member's bytes.
 *
 * Static storage, so the two addresses differ, are stable across runs and are
 * never freed. `static inline` and not `static`: this header reaches a second
 * translation unit that calls none of them, and C11 exempts an unused inline
 * where it would warn on an unused static. */
#include <stdint.h>

typedef union Cell Cell;

union Cell {
    int64_t i;
    double f;
};

static inline Cell *cell_at(int64_t i) {
    static union Cell pool[2] = { { 10 }, { 20 } };
    return &pool[i];
}

static inline Cell *cell_none(void) { return 0; }

static inline int64_t cell_int(Cell *c) { return c->i; }

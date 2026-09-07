/* The C half of `ffi-owned-cell-is-freed.hero` (design.md §4.19; panel 109's
 * conditions 4 and 8).
 *
 * WHY THIS HEADER EXISTS RATHER THAN A LIBRARY. The `@ owned` cell's real
 * witness is SQLite's `sqlite3_exec(…, char **errmsg)`, and `examples/ledger/`
 * is that program. But a machine without libsqlite3 skips it, and the null
 * guard below has no witness in SQLite at all — SQLite is polite and writes a
 * null. Two `static inline` functions in a header the case carries need no
 * `link`, so this runs on every leg of every platform, which is what a
 * memory-ownership rule has to be held to.
 *
 * The allocation is plain `malloc`, so the freer the program names is plain
 * `free`, and LeakSanitizer on the Linux leg is the judge (CLAUDE.md §8).
 */
#ifndef HEROES_OWNED_CELL_IS_FREED_H
#define HEROES_OWNED_CELL_IS_FREED_H

#include <stdlib.h>
#include <string.h>

/* The ordinary shape: allocate a message the CALLER owns, hand it back
 * through the cell, and answer a code. This is `sqlite3_exec`'s contract with
 * nothing else attached to it. */
static inline int hero_owned_describe(int code, char **out) {
    const char *text = (code == 0) ? "all is well" : "something went wrong";
    size_t n = strlen(text) + 1;
    char *copy = (char *)malloc(n);

    if (copy == NULL) {
        *out = NULL;
        return -1;
    }

    memcpy(copy, text, n);
    *out = copy;
    return code;
}

/* THE SHAPE THAT MAKES THE NULL STORE LOAD-BEARING: it is handed a cell and
 * never writes it. A C function that takes an error cell and returns without
 * filling it is ordinary — most libraries fill it only on failure — and
 * without the compiler's own null store before the call, the release would
 * read whatever the stack was holding and hand that to `free`. With it, the
 * program is handed `fail("null_cstr", …)`, which is the truth. */
static inline int hero_owned_silent(int code, char **out) {
    (void)out;
    return code;
}

#endif

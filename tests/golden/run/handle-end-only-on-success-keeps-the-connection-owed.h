/* A close that refuses while a statement is open, returns 5 and leaves the
 * connection open, then succeeds with 0 once the statement is finalized:
 * `sqlite3_close`'s shape. */
#include <stdint.h>
typedef struct db { int64_t open_stmts; } db;
static inline __attribute__((noinline)) db *db_open(void) { static db one; one.open_stmts = 1; return &one; }
static inline __attribute__((noinline)) void db_finalize(db *d) { d->open_stmts--; }
static inline __attribute__((noinline)) int32_t db_close(db *d) { return d->open_stmts > 0 ? 5 : 0; }

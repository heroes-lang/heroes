/* SQLite's open/close pair in miniature over a static pool, for the ffi
 * seat's `sqlite_copies` at panel 177: `db_open` fills an out-cell and
 * answers 0, `db_close` marks the connection closed, `db_changes` reads it.
 * A closed connection reads -1, so a stale copy that reached C would print
 * -1 at exit 0, the silent wrong answer this case exists to refuse. */
#include <stdint.h>

typedef struct cdb cdb;

struct cdb {
    int32_t changes;
};

static inline __attribute__((noinline)) int64_t db_open(const char *path, cdb **out) {
    static cdb pool[4];
    static int64_t next = 0;
    (void)path;
    pool[next].changes = 7;
    *out = &pool[next++];
    return 0;
}

static inline __attribute__((noinline)) int64_t db_close(cdb *db) {
    db->changes = -1;
    return 0;
}

static inline __attribute__((noinline)) int32_t db_changes(cdb *db) { return db->changes; }

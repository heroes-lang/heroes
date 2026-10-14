/* Route I's stand-in for `extern "sqlite3.h"` in examples/sqlite/main.hero,
 * written from the group's Heroes declarations alone: a handle is an
 * incomplete struct under its tag, a constant any integer constant (an
 * ablation unit is compiled with -fsyntax-only and never run), a function
 * the prototype its Heroes types spell. */
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;
enum { SQLITE_OK = 0, SQLITE_ROW = 0 };
int64_t sqlite3_open(const char *a0, sqlite3 **a1);
int64_t sqlite3_close(sqlite3 *a0);
int64_t sqlite3_exec(sqlite3 *a0, const char *a1, void *a2, void *a3, void *a4);
int64_t sqlite3_prepare_v2(sqlite3 *a0, const char *a1, int32_t a2, sqlite3_stmt **a3, const char **a4);
int64_t sqlite3_step(sqlite3_stmt *a0);
int64_t sqlite3_column_int(sqlite3_stmt *a0, int32_t a1);
int64_t sqlite3_finalize(sqlite3_stmt *a0);

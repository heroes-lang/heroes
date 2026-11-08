# Task

Write a complete program, one file `main.hero`, that uses the C library SQLite through its header
`sqlite3.h` and its library `sqlite3`:

1. open the database file `notes.db`;
2. prepare the statement `SELECT 1` on it with `sqlite3_prepare_v2`;
3. close the database while that statement is still prepared, and print the result code the close
   returned;
4. finalize the statement, and print the result code it returned;
5. make sure the database connection is really closed before the program ends, closing it again if
   the first close did not close it, and print every result code a close returns.

The program gives back everything SQLite hands it, and exits 0.

What SQLite's documentation says of the two closes: `sqlite3_close` returns `SQLITE_OK` (0) when it
closed the connection and `SQLITE_BUSY` (5) when a statement is still prepared, and then the
connection stays open; `sqlite3_close_v2` returns `SQLITE_OK` (0), and when a statement is still
prepared the connection is closed once the last one is finalized. Their C declarations, with the two
the program needs beside them:

```
int sqlite3_open(const char *filename, sqlite3 **ppDb);
int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail);
int sqlite3_finalize(sqlite3_stmt *pStmt);
int sqlite3_close(sqlite3 *db);
int sqlite3_close_v2(sqlite3 *db);
```

Write the program under each variant, as your brief says.

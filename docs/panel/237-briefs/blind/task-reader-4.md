# Task

Write a complete program, one file `main.hero`, that uses the C library SQLite through its header
`sqlite3.h` and its library `sqlite3`:

1. open the database file `notes.db`;
2. prepare the statement `SELECT 1` on it with `sqlite3_prepare_v2`;
3. close the database, with that statement still prepared, and print the result code the close
   returned;
4. finalize the statement, and print the result code it returned.

The program gives back everything SQLite hands it, and exits 0. The C declarations it needs:

```
int sqlite3_open(const char *filename, sqlite3 **ppDb);
int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail);
int sqlite3_finalize(sqlite3_stmt *pStmt);
int sqlite3_close(sqlite3 *db);
```

Answer with the program in one fenced code block, and nothing after it.

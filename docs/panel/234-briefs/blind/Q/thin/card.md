# sql: `use core/database/sql`

SQLite's C interface: each function is the C function of that name without `sqlite3_`, with C's parameters in C's order less those the package fills with SQLite's documented default (a string's length, how long SQLite keeps bound text, a statement's tail, and `exec`'s callback, context and message), and answers SQLite's integer result code, which a program compares with the constants below.

## Types

`record Db`: a database connection, C's `sqlite3 *`; `nullptr` until `open` writes it, and owed one `close` once `open` has run, even where `open` failed.

`record Stmt`: a prepared statement, C's `sqlite3_stmt *`; `nullptr` until `prepare_v2` writes it, and owed one `finalize` where it did.

A handle still owed its `close` or `finalize` when `main` returns aborts the program, and so does a handle used after it was given back.

## Functions

- `open(filename: str, @db: Db) -> i64`: opens the database `filename` names, `":memory:"` for one held in memory, into `db`; answers `SQLITE_OK` or an error code.
- `close(@db: Db) -> i64`: closes `db`; answers `SQLITE_OK`, or an error code while a statement of `db` is not finalized, and then closes nothing.
- `exec(db: Db, sql: str) -> i64`: runs every statement in `sql`; answers `SQLITE_OK` or an error code.
- `prepare_v2(db: Db, sql: str, @statement: Stmt) -> i64`: compiles the first statement in `sql` into `statement`; answers `SQLITE_OK` or an error code, `statement` then staying `nullptr`. Text after the first statement is not compiled.
- `bind_text(statement: Stmt, index: i32, text: str) -> i64`: binds `text` to the `index`th `?` of `statement`, counted from 1; answers `SQLITE_OK` or an error code. SQLite keeps its own copy, and a column declared `INTEGER` stores a text that reads as an integer as that integer.
- `step(statement: Stmt) -> i64`: runs `statement` to its next row; answers `SQLITE_ROW` when a row is ready, `SQLITE_DONE` when it has finished, `SQLITE_CONSTRAINT` when a constraint such as `UNIQUE` refused it, or another error code. After anything but `SQLITE_ROW`, `reset` it before stepping it again.
- `reset(statement: Stmt) -> i64`: makes `statement` ready to step again from its start, keeping its bindings; answers `SQLITE_OK`, or the code of its last `step` where that step failed.
- `finalize(@statement: Stmt) -> i64`: ends `statement`; answers `SQLITE_OK`, or the code of its last `step` where that step failed and no `reset` came after it.
- `column_int64(statement: Stmt, column: i32) -> i64`: the integer in column `column`, counted from 0, of the row `step` made ready; a NULL reads 0.
- `column_text(statement: Stmt, column: i32) -> str?`: the text in column `column`, counted from 0, of the row `step` made ready; fails `null_cstr` or `not_text`.

## Constants

- `SQLITE_OK`, `SQLITE_ROW`, `SQLITE_DONE`, `SQLITE_CONSTRAINT`, each an `i64`: SQLite's result codes, their values read from `sqlite3.h`.

## Codes

- `null_cstr`: the column holds NULL.
- `not_text`: the column's bytes are not UTF-8.

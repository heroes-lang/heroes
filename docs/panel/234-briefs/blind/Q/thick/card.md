# sql: `use core/database/sql`

SQLite, with every failure a `T?`: no status code and no prepared statement reaches the program.

## Types

`record Db`: an open database, from `open`, owed one `close`; its field is the package's own.

`record Result` with `rows_affected: i64` and `last_insert_id: i64`: what `exec` changed: the rows its statement inserted, changed or deleted, and the row id of the last row the database inserted.

`variant Value` with the cases `integer` (`value: i64`), `real` (`value: f64`), `text` (`value: str`) and `null_value`: one column's value, `null_value` being SQL's NULL.

`record Row` with `values: [Value]`: one row of a query's result, its columns in the query's order, from 0.

A `Db` still owed its `close` when `main` returns aborts the program, and so does a `Db` used after its `close`.

## Functions

- `open(data_source_name: str) -> Db?`: the database `data_source_name` names, a file's path, or `":memory:"` for one held in memory; fails `cannot_open`.
- `close(@db: Db)`: closes `db`.
- `exec(db: Db, sql: str, arguments: [str]) -> Result?`: runs the first statement in `sql`, each `?` in it bound to the next of `arguments` as text, and answers what it changed; fails `refused` or `unique`. A column declared `INTEGER` stores a text that reads as an integer as that integer.
- `query(db: Db, sql: str, arguments: [str]) -> [Row]?`: runs the first statement in `sql`, bound as `exec` binds it, and answers every row it gives, in order; fails `refused`, `unique`, `not_text` or `wrong_type`.
- `integer(row: Row, column: i64) -> i64?`: the integer in column `column` of `row`, counted from 0; fails `null_value` or `wrong_type`.
- `text(row: Row, column: i64) -> str?`: the text in column `column` of `row`, counted from 0; fails `null_value` or `wrong_type`.

## Codes

- `cannot_open`: SQLite could not open the database; the message says why.
- `refused`: SQLite refused the statement, for its syntax, a name it does not know, a constraint other than `UNIQUE`, or a number of `arguments` other than its `?`s; the message is SQLite's.
- `unique`: the statement would write a value that a `UNIQUE` or primary-key column already holds; it wrote nothing.
- `null_value`: the column holds NULL.
- `wrong_type`: the column holds another type than the one asked for; from `query`, a column holds a blob, which this package does not read.
- `not_text`: a text column's bytes are not UTF-8.

# minidb

minidb: a small SQL database engine, not a client. Its SQL is read by a
lexer on Rebel's `scanner` and a recursive-descent parser: `CREATE TABLE`,
`CREATE [UNIQUE] INDEX`, `DROP`, `INSERT` of values or of a query,
`UPDATE`, `DELETE`, and `SELECT` with inner and left joins, `WHERE`,
`GROUP BY`, `HAVING`, `ORDER BY`, `LIMIT`, `OFFSET`, `DISTINCT`, the
aggregates, `CASE`, `IN`, `BETWEEN`, `LIKE`, and `NULL` by SQL's
three-valued logic. Types are strict, as PostgreSQL's are; an integer
overflow, a division by zero and a type mismatch are errors, never a
wrapped or a guessed value.

Its storage is SQLite's design in miniature: a file of fixed-size pages
read through a page cache; a B+tree for every table, keyed by rowid, and
for every index; a catalogue kept in its own tree, the `CREATE` statements
parsed again at every open; a write-ahead log where a transaction's pages
are appended, each frame checksummed and chained to the one before, and
recovered after a crash, the frames of a transaction that never committed
cut off; a checkpoint that copies the log into the database file. One
connection writes at a time, under a lock on a file beside the database;
the others read their snapshot and never wait. A planner chooses a search
of the rowid or of an index over a scan of the whole table, and `EXPLAIN`
shows its choice in SQLite's words. A connection answers the contract of
Rebel's `sql` drivers, `exec`, `query`, `begin`, `commit` and the rest.

Run for real:

    minidb shell shop.db                 # statements and dot-commands on stdin
    minidb shell shop.db --echo < a.sql  # a script, each statement echoed
    minidb exec shop.db "SELECT 1 + 1;"  # the statements of one argument

`minidb demo` plays the script of the standard input on a fresh
`build/minidb-demo/shop.db` with small pages: a shop's tables, a few
hundred orders made by a join, the trees they make, joins, groups and
three-valued logic, `EXPLAIN`, broken constraints, a transaction rolled
back, a second connection kept out by the writer's lock, a crash after a
commit and a crash in the middle of a transaction, each followed by the
recovery of its log, an integrity check and a checkpoint.

The packages of Rebel it uses, and what for:

- `scanner`, `regex`: the lexer, SQL's quotes read by patterns;
- `sql`: its values, rows and results, and the drivers' contract;
- `bytes`, `hash`: the pages, the rows and the keys as bytes, and the
  log's CRC-32;
- `containers/cache`: the page cache, which forgets the page used least
  recently;
- `fs`, `io`, `path`: the files, read and written by place, synced and
  locked;
- `math`, `numbers`, `strings`, `lists`: arithmetic that refuses an
  overflow, numbers read and written, text, sorting by `ORDER BY`;
- `csv`, `json`: the shell's `.mode csv` and `.mode json`;
- `flags`, `errors`: the command line, and failures told.

## Its demo

Run from the repository's root with the words of `main.args` and `main.stdin` on its input, as the check of Rebel's programs runs it (`tests/harness/rebel_apps.hero`), it plays a whole session on this machine and prints `main.expected` byte for byte:

    heroes run rebel/examples/minidb/main.hero -- demo < rebel/examples/minidb/main.stdin

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 17 faces on its `packages` line, each linked here to its reference: [`bytes`](https://heroes-lang.org/rebel/bytes/), [`containers/cache`](https://heroes-lang.org/rebel/containers/cache/), [`csv`](https://heroes-lang.org/rebel/csv/), [`errors`](https://heroes-lang.org/rebel/errors/), [`flags`](https://heroes-lang.org/rebel/flags/), [`fs`](https://heroes-lang.org/rebel/fs/), [`hash`](https://heroes-lang.org/rebel/hash/), [`io`](https://heroes-lang.org/rebel/io/), [`json`](https://heroes-lang.org/rebel/json/), [`lists`](https://heroes-lang.org/rebel/lists/), [`math`](https://heroes-lang.org/rebel/math/), [`numbers`](https://heroes-lang.org/rebel/numbers/), [`path`](https://heroes-lang.org/rebel/path/), [`regex`](https://heroes-lang.org/rebel/regex/), [`scanner`](https://heroes-lang.org/rebel/scanner/), [`sql`](https://heroes-lang.org/rebel/sql/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

10 folders, 34 files and 9,056 lines, this page aside; of them 30 modules, `.hero` files, of 8,684 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 5 | 508 | 1 | 136 |
| `catalog/` | 1 | 281 | 1 | 281 |
| `cli/` | 1 | 140 | 1 | 140 |
| `db/` | 1 | 540 | 1 | 540 |
| `demo/` | 1 | 47 | 1 | 47 |
| `engine/` | 9 | 2,330 | 9 | 2,330 |
| `shell/` | 2 | 554 | 2 | 554 |
| `sql/` | 6 | 1,715 | 6 | 1,715 |
| `storage/` | 5 | 2,105 | 5 | 2,105 |
| `values/` | 3 | 836 | 3 | 836 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.

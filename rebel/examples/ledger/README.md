# ledger

The twin of `examples/ledger/`: a ledger kept in SQLite, and every number
checked twice, written with Rebel. It prints byte for byte what
`examples/ledger/` prints, so that program's `main.expected` is this one's
test too.

    heroes run rebel/examples/ledger/main.hero

    main.hero           the program: the database, the queries, the check
    book/entry.hero     the book, the accounts, and the arithmetic in cents
    report/lines.hero   the report's text

HOW IT IS CHECKED. There is no published answer for a book somebody
invented, so the second opinion is built: **every total is computed twice,
by two routes that share no line**, once by SQLite's own
`sum(...) group by ...` and once by Heroes over the same rows, and the
program prints `yes` only where they match cent for cent.

What Rebel does here that `examples/ledger/` writes for itself:

- `sql/sqlite` opens the database in memory, runs a statement with its
  values bound to placeholders, prepares one statement and runs it many
  times, and answers a query's rows as `sql.Row`s, in place of that
  program's `db/sqlite.hero` (its eighteen bindings of `sqlite3.h`, its
  handles, its cell reader, and its care that a statement is finalized and
  an error message freed on every path);
- `sql.integer_at`, `sql.text_at` and `sql.value_at` read a row's values by
  their type, refusing a text read as a number rather than answering zero;
- `strings.pad_end` and `strings.pad_start` pad a column, in place of the
  loops in its `report/lines.hero`; `strings.contains`, in the tests, in
  place of its `strings.hero`.

What stays this program's own is the book, the money in cents, the report,
and the check that holds SQLite's answers to Heroes' own.

## Its original

The twin of [`examples/ledger/`](../../examples/ledger/), the compiler's own program, which takes no library. Run from the repository's root with no word, as that program is, it prints that program's `main.expected` byte for byte, and the check of Rebel's programs holds it to it (`tests/harness/rebel_apps.hero`):

    heroes run rebel/examples/ledger/main.hero

## The packages it uses

Its catalogue, `heroes.toml`, reads Rebel from `../../library/0.1.0/files` and lists 3 faces on its `packages` line, each linked here to its reference: [`sql`](https://heroes-lang.org/rebel/sql/), [`sql/sqlite`](https://heroes-lang.org/rebel/sql/sqlite/), [`strings`](https://heroes-lang.org/rebel/strings/).

## Its folders

3 folders, 4 files and 600 lines, this page aside; of them 3 modules, `.hero` files, of 597 lines.

| folder | files | lines | modules | their lines |
|---|---:|---:|---:|---:|
| `./` | 2 | 297 | 1 | 294 |
| `book/` | 1 | 228 | 1 | 228 |
| `report/` | 1 | 75 | 1 | 75 |

---

This page is written by `rebel/generators/rebel_index/`, never by hand, from the program's own files: its `main.hero`'s opening comment, its catalogue `heroes.toml`, its `main.args` and what its folders hold. Change those, then run `heroes run rebel/generators/rebel_index/main.hero -- write ./heroes` from the repository's root; `check` in place of `write` says whether this page is still what they make.

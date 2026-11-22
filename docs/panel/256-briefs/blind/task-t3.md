# Task

Write `main.hero`, a whole program, beside this module `db.hero`:

```
extern "sqlite3.h" link "sqlite3"
    constant SQLITE_OK: i64
    record Db tag sqlite3
    function sqlite3_open(path: cstr lent, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64 when 0

function open_memory() -> Db
    db: Db @= nullptr
    _ = sqlite3_open(":memory:".cstr(), @db)
    return db

function shut(@db: Db) -> bool
    return sqlite3_close(db) == SQLITE_OK
```

`main` starts two **workers**, each a thread of its own that takes one value from a mailbox of its own,
waiting at most five seconds, and answers:

- the **counter**: takes an `i64` and answers it plus one;
- the **closer**: takes an open `db.Db` and answers whether `db.shut` closed it.

`main` sends `41` to the counter and a database it opened with `db.open_memory()` to the closer. **Where the
language allows it, both sends go through one generic function of your own, `deliver`**, that sends one
value into a mailbox and answers whether the send failed; where it does not, say why and write what it
does allow. Then `main` waits for both workers and prints

    counter: 42
    closer: true

and the program ends normally, not by an abort, with every thread it started waited for and the database
closed once.

Give `main.hero` whole, then say whether `heroes build main.hero` accepts it, every line the program
writes, and how it ends.

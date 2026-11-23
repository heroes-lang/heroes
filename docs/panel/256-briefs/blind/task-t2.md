# Task

Below are a module `db.hero` and seven programs, each a whole `main.hero` beside that module. For each
program, in order, say:

1. whether `heroes build main.hero` accepts it, and where it does not, which line it refuses and why;
2. where it is accepted, every line the program writes, in order;
3. how it ends: normally, or by an abort and what the abort says.

Each is run once with no arguments, on a machine where `sqlite3.h` and its library are installed.

## `db.hero`

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

## Program 1

```
function relay<T>(m: (mailbox T), v: T) -> bool
    return m.send(v).is_err()

function main()
    m: (mailbox i64) = mailbox()
    print(relay(m, 7))
    print(m.receive(within_ms: 0).must())
```

## Program 2

```
use db

function hand<T>(m: (mailbox T), @v: T) -> bool
    return m.send(@v).is_err()

function main()
    m: (mailbox db.Db) = mailbox()
    d @= db.open_memory()
    print(hand(m, @d))
    got @= m.receive(within_ms: 0).must()
    print(db.shut(@got))
```

## Program 3

```
use db

function relay<T>(m: (mailbox T), v: T) -> bool
    return m.send(v).is_err()

function main()
    m: (mailbox db.Db) = mailbox()
    d @= db.open_memory()
    print(relay(m, d))
    got @= m.receive(within_ms: 0).must()
    print(db.shut(@got))
    print(db.shut(@d))
```

## Program 4

```
function hand<T>(m: (mailbox T), @v: T) -> bool
    return m.send(@v).is_err()

function main()
    m: (mailbox i64) = mailbox()
    n @= 7
    print(hand(m, @n))
    print(m.receive(within_ms: 0).must())
```

## Program 5

```
use db

function start<S>(body: (function(S) -> bool), setup: S) -> bool
    t = spawn(body, setup).must()
    return t.wait()

function close_it(d: db.Db) -> bool
    mine @= d
    return db.shut(@mine)

function main()
    d @= db.open_memory()
    print(start(close_it, setup: d))
    print(db.shut(@d))
```

## Program 6

```
function start<S>(body: (function(S) -> i64), setup: S) -> i64
    t = spawn(body, setup).must()
    return t.wait()

function twice(n: i64) -> i64
    return n * 2

function main()
    print(start(twice, setup: 21))
```

## Program 7

```
use db

function inner<T>(m: (mailbox T), v: T) -> bool
    return m.send(v).is_err()

function outer<A>(m: (mailbox A), v: A) -> bool
    return inner(m, v)

function main()
    m: (mailbox db.Db) = mailbox()
    d = db.open_memory()
    print(outer(m, d))
    got @= m.receive(within_ms: 0).must()
    print(db.shut(@got))
```

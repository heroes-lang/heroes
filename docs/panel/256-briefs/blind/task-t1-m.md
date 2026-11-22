# Task

Write `main.hero`, a whole program, using the package described below beside the language's own
specification (`heroes-spec.md`).

## The package `poll`, and what it uses (`use <rebel>/poll`, `use <rebel>/net`, `use <rebel>/time`)

These functions come from a package the program's catalogue already names; each is written in Heroes.

- `net.listen(host: str, port: i64) -> net.Listener?`: a listener on `host` at `port`, `0` for any free
  port. A `net.Listener` is a record holding a C handle (section 13 of the specification).
  `net.close_listener(@l: net.Listener) -> ()?` closes it.
- `time.milliseconds(n: i64) -> time.Duration?` and `time.seconds(n: i64) -> time.Duration?`: a duration;
  each fails `invalid` below 0.
- `poll.make(most: i64) -> poll.Set?`: a set that waits on at most `most` entries at once. A `poll.Set` is
  a record holding a C handle. `poll.close(@set: poll.Set) -> ()?` closes it.
- `poll.add_listener(set: poll.Set, l: net.Listener) -> i64?`: adds the listener, told readable when a
  connection waits to be accepted, and answers the entry's key, a number above 0.
- `poll.add_mailbox<T>(set: poll.Set, m: (mailbox T)) -> i64?`: adds the mailbox `m` (section 11 of the
  specification), told readable while it holds a value, is closed, or answers `stopped`, and answers the
  entry's key.
- `poll.wait(set: poll.Set, limit: time.Duration) -> [poll.Event]?`: waits until at least one entry is
  ready or `limit` passes, and answers one `poll.Event` per entry ready (empty where `limit` passed with
  none). `poll.Event` is a record whose field `key: i64` names the entry. **Level-triggered**: what stays
  ready is told again at every wait until the program takes it.

## The program

`main` makes a set holding a listener on `127.0.0.1`, port `0`; nobody connects to it in this run. It
starts four **workers**, numbered 0 to 3, each a thread of its own. Worker `w` sends `main` 25 results,
`i` from 0 to 24, each the record `Result(worker: w, value: w * 1000 + i)`, through a mailbox, and its
thread answers how many it sent.

`main`'s thread **waits only in `poll.wait`**, never in a mailbox's `receive` with a limit above 0, and
with a `limit` of 30 seconds, so a result must wake the wait, never the limit. Each time it is woken it
prints every result that has arrived, one line each:

    got <worker> <value>

Once it has printed all 100, it prints

    done 100

waits for every worker, closes what it opened, and the program ends normally, not by an abort and not by
waiting for ever. The order of the `got` lines among workers does not matter.

Give `main.hero` whole, then say whether `heroes build main.hero` accepts it, every line the program
writes (the `got` lines as how many, from which workers), and how it ends.

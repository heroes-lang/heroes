# Notes

- **`struct counter` as `record Counter tag counter partial` with only `n: i64`.** The struct
  contains a `pthread_mutex_t`, which no allowed field type can describe. So `partial` leaves the
  lock out, and the record keeps C's full size. Other choice: listing `lock` is refused.
- **Setting up the counter in place: `setup(@c: Counter) -> ()?`.** The other choice was
  `new_counter() -> Counter`, but returning the value moves it after `counter_init`. Per §13 C may
  only keep a place that doesn't move, and a moved pthread mutex is undefined behaviour.
- **C parameters are `@c: Counter lent`** for init, lock and unlock, because C writes but doesn't
  keep the pointer. Without `lent`, C is assumed to keep the address. `counter_value` uses
  `c: Counter lent` without `@`, which means a pointer to const. That matches the header and keeps
  the counter unchanged.
- **`doubled` takes `c: Counter lent`** so it reads the counter without copying it. A plain
  parameter would be a copy of the bytes, mutex included.
- **Integer results declared `i32`** (C's `int`). A result may be wider, so `i64` would also pass.
- **Every C status is checked.** Nonzero becomes a `fail` with code `init_failed`, `lock_failed`
  or `unlock_failed`. `main` returns nothing, so it can't use `?` and calls `.must()`, which
  aborts on an error. Other choice: `_ =` on an `i32` would quietly ignore errors.
- **`link "pthread"`.** On newer glibc and macOS the pthread functions are in libc, so dropping it
  would probably also work.
- **Records start as `Counter(n: 0, rest: zero)`.** This zeroes the mutex bytes before
  `counter_init` runs.
- **Repetition uses `while` with a counter cell.** I avoided `for _ in range(...)` because the
  grammar takes an `ident` after `for` and may not accept `_` there. Writing the calls out five
  times would also work.
- **Events are counted in `main`** after each increment. The other choice was a helper taking two
  `@` arguments, `@s.first` and `@s.events`, which depends on the rule about separate places.

context: yes — the system prompt reached me: I'm a Claude agent built on the Claude Agent SDK, plus
the working directory, the date (2026-10-16) and the rule about calling tools. The user message
also said to write answers in this directory in English. I only had a file-reading tool, so I
couldn't write the files and gave their contents in my reply instead. No repository status or
memory reached me.

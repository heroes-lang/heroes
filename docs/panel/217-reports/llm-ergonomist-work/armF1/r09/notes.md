# Notes

- **`struct counter` as `record Counter tag counter partial` with only `n: i64`.**
  `pthread_mutex_t` can't be written as a field type, so `partial` leaves it out and the
  size stays C's. Listing every field isn't possible. Cost: a `Stats` can't be compared
  with `==` or used as a map key.
- **Result widths: `i32` for C's `int`.** A result may be wider than C's, so `i64` would
  also be accepted; nothing else changes.
- **`@c: Counter lent` on init/lock/unlock.** The inline functions don't keep the pointer.
  Without `lent`, C is taken to keep the place, which is allowed only while it doesn't
  move. The program would behave the same, but the declaration would be less accurate.
- **`counter_value(c: Counter lent)`.** `const struct counter *` is a pointer to const,
  so `lent` without `@` is the matching form. Using `@` would claim C writes to it and
  would make callers pass a mutable place.
- **The setup function takes `@c: Counter` and inits the caller's place.** The other
  choice, `-> Counter` that inits a local and returns it, moves an initialised
  `pthread_mutex_t`. Copying an initialised mutex is undefined in POSIX. The counter is
  first built with `Counter(n: 0, rest: zero)`, which zero-fills the mutex bytes before
  `counter_init`.
- **`doubled(c: Counter lent)` rather than a plain copy parameter.** It reads the
  caller's counter in place and copies no mutex. A copy would be legal but would
  duplicate the mutex bytes.
- **Checking C results with `assert ... == 0`.** The other choices are `_ = ...`, which
  silently ignores lock errors, or a `()?` return, which every caller would then have to
  handle. Aborting on a failed init or lock seemed right for this program.
- **Event counting is written out in `main`, with no loop.** `for _ in range(...)` relies
  on `_` being allowed as a loop variable, and the grammar says `ident`. A named loop
  variable would be an unused binding, which is an error.
- **Events are counted in `main` next to each `increment`.** They could instead go in a
  helper taking `@s: Stats`. The output is the same.

context: yes. Besides the files in this folder, I received the system framing: I'm
described as a Claude agent with a working-directory path, a date (2026-10-16), and an
instruction about making tool calls in parallel. No repository status, memory, or other
rules reached me. The only tool I had was a file reader, so these files are given as text
rather than written to disk.

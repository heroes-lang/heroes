# Notes

- **`struct counter` is `partial`, naming only `n`.** `pthread_mutex_t` isn't a type a group
  field can have. Other choice: list every field. That gets refused.
- **The counter is set up in place: `setup(@c: Counter)`, not one that returns a `Counter`.**
  A return moves the value, and a pthread mutex must not move once it is initialised. Other
  choice: return a new `Counter`. That copies an initialised mutex, which POSIX leaves undefined.
- **The caller builds each counter first with `Counter(n: 0, rest: zero)`.** Other choice: name
  no field and only write `rest: zero`. The spec doesn't clearly allow that.
- **`counter_init`, `counter_lock` and `counter_unlock` are `@c: Counter lent`.** C writes
  through the pointer but doesn't keep it, so an `@ s.first` field can be lent. Other choice:
  leave out `lent`. Then C is taken to keep the place, and lending a field gets refused.
- **`counter_value` is `c: Counter lent`, which the spec reads as a pointer to const.** That
  matches the header's `const struct counter *`. Other choice: `@`. That claims a write that
  doesn't happen, and the doubling function would then need an `@` it never writes.
- **Results are declared `i32`, C's own width.** Other choice: `i64`, which the spec allows
  because a result may be wider.
- **A non-zero result becomes `fail(...)`, and `main` calls `.must()`.** Other choice: ignore
  the result. That's a compile error, because an unused value isn't allowed.
- **No `link` clause.** The counter functions are `static inline`, and `pthread_mutex_*` is in
  libc on macOS and on glibc 2.34 or later. Other choice: `link "pthread"`. Older glibc needs
  it; elsewhere it does no harm.
- **The loops are `while` with counter cells.** `for _ in range(...)` would need `_` to be
  allowed as the loop name, and the grammar there only says `ident`. Naming the loop variable
  `i` would be an unused binding, which is an error.
- **Events are counted in `main` after each increment.** Other choice: count them inside a
  helper that takes `@s: Stats`. The output is the same.

context: yes. Besides the folder's files, I received a system prompt saying I am a Claude agent
built on the Claude Agent SDK. It gave the working directory and the date (2026-10-16), and
offered only a file-reading tool, so I couldn't write these files myself. I received no
repository status, memories, or project rules.

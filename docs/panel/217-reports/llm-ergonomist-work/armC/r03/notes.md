# Notes

- **`struct counter` as a `partial` record naming only `n`.** `pthread_mutex_t` is not a type a
  field may have (number, bool, ptr, cstr, group record, fixed array). Naming every field is
  impossible, so `partial` is the only way. Consequence: a `Stats` can't be compared or used as
  a map key.
- **"Sets up a new counter for its caller" fills the caller's place through an `@` parameter
  instead of returning a `Counter`.** A return moves the value, and a pthread mutex must not
  move after `pthread_mutex_init`. Returning one would compile but copy an initialised mutex,
  which POSIX leaves undefined. The caller first builds a zeroed placeholder
  (`Counter(n: 0, rest: zero)`) inside `Stats`, then initialises it where it lives.
- **Every C parameter is declared `lent`.** None of the header's functions keep the pointer.
  Without `lent` the parameter counts as kept, and the lend rules would refuse the call.
  `counter_value` is `lent` without `@`, which makes it the header's `const struct counter *`.
- **The `int` results are `i32` and checked.** The setup and increment functions return `()?`
  with a `fail` on a non-zero result, and `main` calls `.must()`. Other options: `i64` results
  (allowed, since a result may be wider), or `_ =` to ignore them (silently loses lock errors).
- **`doubled` takes `Counter lent`, not a plain `Counter`.** A plain parameter would copy the
  record, mutex included.
- **The `n` increment is Heroes code (`c.n @ c.n + 1`) between `counter_lock` and
  `counter_unlock`, as the task asks.** It writes the caller's place directly.
- **The loops are `while` with counter cells, not `for _ in range(...)`.** The spec doesn't say
  whether `_` is allowed as a `for` variable, and a named variable that is never read is an
  error. Writing out all five calls would also work, just with more lines.
- **No `link "pthread"`.** On current glibc and macOS the pthread symbols are in libc. On an
  older glibc this program would fail to link, and `link "pthread"` would fix it.
- **The mutexes are never destroyed.** The header has no `pthread_mutex_destroy` wrapper, and
  the mutexes are not handles, so nothing is owed at exit.
- **Error codes are my own names:** `counter_init_failed`, `counter_lock_failed`,
  `counter_unlock_failed`.

context: yes. Besides the folder's files, I received a system prompt saying I am a Claude agent
built on the Claude Agent SDK, with a working directory, today's date (2026-10-16) and
tool-use rules, plus the user's message telling me to follow brief.md. No repository status,
memory or extra language rules reached me. My only tool could read files but not write them,
so these files had to be given as text instead of written to the folder.

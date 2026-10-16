# Notes

- **How `doubled` gets the counter.** The compiler refused copying a `Counter` (its struct holds a
  `pthread_mutex_t`): the by-value parameter, `view @= c`, and the two calls. Choice: `doubled(@c: Counter)`,
  called as `doubled(@stats.first)`, as the compiler's fix suggests. Other choice: return `c.n * 2`
  (the compiler's "a field is read in place") with the same `@c` parameter. That compiles too, but
  skips `counter_value`, which the task requires.
- **A read-only `@` parameter.** The spec rejects an `@` parameter that nothing writes, but counts an `@` argument
  as a write, so passing `@c` to `counter_value` keeps `doubled` legal. Nothing changes the counter:
  C's parameter is `const`. Other choice: declare `counter_value(c: ptr ...)` and lend with
  `.ptr()`. The spec only allows that for a field or a record "lent whole with `@`" and
  `counted_by`, which needs more guessing for no gain.
- **`counter_value`'s declaration.** Kept as `@c: Counter lent` even though C's pointer is `const`.
  The spec has no word for const. Without `lent`, C would be taken to keep the place it is handed,
  and a borrowed place can't be given for keeping.
- **`setup` writes into the caller's place (`@c`).** It doesn't return a new `Counter`, because a return
  moves the value and the mutex would be initialised somewhere else first. The compiler didn't
  object to this part of the old program, so I kept it.
- **`Counter(rest: zero)` inside `Stats(...)`.** Unchanged. Line 39 wasn't flagged, so I took
  building the value in place as not a copy.
- **Failed `counter_*` calls.** Kept: print the code and `exit(code: 1)`. Other choice: ignore the
  codes with `_ =`. That compiles, but a lock failure would go unnoticed.
- **No `pthread_mutex_destroy`.** The header doesn't declare it and `Counter` isn't a handle, so
  nothing is owed when `main` returns.

context: yes — besides the folder's files, I received a system prompt saying I am a Claude agent
built on the Claude Agent SDK, the working directory, the date (2026-10-16), and a tool list with
only a file-reading tool (no way to write files). No memories, extra rules or repository status
reached me.

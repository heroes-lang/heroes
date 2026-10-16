# Notes

- **`partial` record with `tag counter`.** `pthread_mutex_t` is not an allowed field type, so the
  record names only `n` and keeps C's size. The header writes `struct counter` with no typedef, so
  `tag counter` is needed. Alternative: listing `lock` would be refused.
- **Setup works in place (`@c: Counter`), not by returning a `Counter`.** A return or copy moves the
  struct, and copying a mutex after `pthread_mutex_init` is undefined in POSIX. Alternative: a
  function returning a fresh `Counter` would read better but would move an initialised mutex.
- **Counters start as `Counter(n: 0, rest: zero)` inside the `Stats` construction**, and are then
  initialised through `@s.first` / `@s.second`. This assumes a field place can be an `@` argument;
  the spec implies it ("works on the caller's place itself") but never shows it.
  Alternative: put each counter in its own cell, initialise it, then build `Stats`. That would copy
  initialised mutexes.
- **`counter_value` takes `c: Counter lent`** because the header says `const struct counter *`
  (pointer to const). Alternative: `@c` would compile but would allow writes and need `@` at every
  call site.
- **Results are checked with `assert rc == 0`.** The `int` results have to be used somehow.
  Alternatives: `_ = counter_lock(@c)` drops errors without a word; returning `()?` with
  `fail(...)` would push `.must()` or `?` into `main`.
- **`link "pthread"`.** The spec says to link a library when the symbols need one. Alternative:
  leaving it out works on glibc ≥ 2.34 and macOS, but fails to link on older glibc.
- **`while` loops with counter cells instead of `for _ in range(...)`.** The grammar's `For` takes an
  `ident`, and it's unclear whether `_` counts as one; a named loop variable would be an unused
  binding, which is an error. Alternative: five written-out calls, which also works.
- **The event count is updated inline in `main`.** Alternative: a helper taking `@s: Stats`, which
  behaves the same.
- **No `pthread_mutex_destroy`.** The header doesn't offer one, so the mutexes are never destroyed.

context: yes — a system prompt reached me. It said I am a Claude agent built on the Claude Agent
SDK and gave the working directory, the date (2026-10-16) and tool-use instructions. The user
message pointed me to brief.md. I had no repository status or memory. Only the Read tool was
available, so I could not write these files myself.

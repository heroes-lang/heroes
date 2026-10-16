# Notes

- **Record declaration**: `record Counter tag counter partial` with only `n: i64`. The header
  writes the name after `struct`, so `tag` is needed. `pthread_mutex_t` is not a type a field can
  have, so it is left to `partial`. Other choice: list the lock field, which would be refused.
- **No `link`**: the pthread functions are in libc on current systems. Other choice:
  `link "pthread"`, which would be needed on older glibc.
- **Result widths**: the `int` results are declared `i32`, the header's own width. Other choice:
  `i64`, which is also allowed because a result may be wider.
- **Pointer parameters**: `struct counter *` is an `@` parameter, and `const struct counter *` is
  `lent` without `@` (a pointer to const). Other choice: a plain parameter, which would pass a copy
  and so the wrong place.
- **"Sets up a new counter for its caller"**: `setup(@c)` initialises the caller's own place. Other
  choice: return a `Counter`. A return moves the value, and a moved `pthread_mutex_t` is undefined
  behaviour in POSIX.
- **Building the record before init**: `Counter(n: 0, rest: zero)`, so the lock starts zeroed.
  Other choice: `Counter(rest: zero)` alone. The spec does not say plainly that this is allowed.
- **C's return codes**: a nonzero result becomes a `fail`, and `main` calls `.must()`, which
  aborts. Other choice: `_ = counter_init(@c)` ignores failures and fails silently.
- **Passing fields**: `@s.first` passes a field of a cell, and C writes back because `s` is a `@`
  name. Other choice: separate cells for each counter, which would move them into `Stats` after
  init.
- **Loops**: `while` with counter cells `i` and `j`, because the grammar's `for` takes an `ident`
  and it is unclear whether `_` counts as one. Other choice: `for _ in range(from: 0, to: 3)`,
  which is shorter if `_` is allowed there.
- **Call style**: `doubled(s.first)` rather than the UFCS form `s.first.doubled()`. Both mean the
  same.
- **Expected output**: `6`, `4`, `5`.

context: yes. The run had a system prompt and a working-directory path. The path looks like a
repository scratchpad (`.../heroes-lang/.../217-llm-ergonomist/armF1/r02`). I was also told the
date. No repository status, memory or extra rules about the language came with it. All of the
content came from brief.md, heroes-spec.md, task.md and counter.h.

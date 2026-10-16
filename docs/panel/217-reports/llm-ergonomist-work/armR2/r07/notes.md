# Notes

- **How `doubled` receives the counter.** The compiler refused the by-value `c: Counter`
  (and the copies `local @= c` and `doubled(s.first)`) because `struct counter` holds a
  `pthread_mutex_t`. I followed its suggested fix: `doubled(@c: Counter)`, called as
  `doubled(@s.first)`. Another choice, reading `c.n` directly, would be allowed but would not
  use `counter_value`, which the task requires. Keeping a copy anywhere would hit the same error again.
- **Does an `@` parameter that is only read count as written?** Section 5 counts an `@`
  argument as a use. So passing `@c` on to `counter_value(@c)` should satisfy the
  "`@` parameter nothing writes" rule. If the compiler disagreed, `doubled` would be refused
  as an unwritten `@` parameter. Since `counter_value` takes a `const` pointer in C, the
  counter really is unchanged.
- **Everything else is unchanged from `previous.hero`.** The compiler raised no other
  errors, so I kept `partial` with only `n` (the mutex is left to C), `rest: zero`
  construction, `lent` on each `@c`, `()?` results with `.must()`, and `while` loops.
  Choosing `for _ in range(...)` instead would risk `_` not being an `ident` in `For`.
- **Failure codes.** Returning a `()?` from `setup` and `increment` is my choice. Ignoring
  the C result would require discarding an `i32`. Calling `.must()` aborts on a non-zero
  result instead of carrying on.
- **Places do not move.** `s` is built once and never copied after `counter_init`, so the
  mutex addresses C sees stay valid.

Expected output: `6`, `4`, `5`, one per line.

context: yes. The only thing that reached me from outside the files was the environment
setup: the working directory path, a date (2026-10-16), and the note that I am an agent
built on the Claude Agent SDK with file-reading only. No other instructions, rules,
memories or repository status.

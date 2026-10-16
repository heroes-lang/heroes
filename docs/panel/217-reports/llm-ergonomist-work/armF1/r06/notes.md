# Notes

- **Counter record: `partial`, naming only `n`.** `pthread_mutex_t` is not a number, bool, ptr, cstr
  or group record, so the spec offers no way to declare it as a field. Other choice: list `lock` as
  a field, which would be refused.
- **Record name `Counter tag counter`.** The header writes `struct counter`, so `tag` is needed;
  the Heroes name is my pick. Other choice: `record counter` with no tag would also match, but
  breaks my naming convention.
- **"Sets up a new counter for its caller" is an `@` out-parameter (`setup_counter(@c)`), not
  `-> Counter`.** Returning the counter moves it, and moving an initialised pthread mutex is
  undefined in POSIX. Other choice: build and return a Counter. That matches the wording more
  literally but moves the mutex. It needs a zeroed value first: `Counter(n: 0, rest: zero)`.
- **`lent` on all four C parameters.** None of these C functions keeps the pointer; without `lent`,
  C is assumed to keep it, so no field place could be passed. `counter_value` uses `c: Counter lent`
  without `@`, which the spec defines as a pointer to const, matching the header. Other choice: `@`
  on `counter_value`, which would let C write and disagree with the `const`.
- **Integer results declared `i32`** (C's int). The spec allows a wider result, e.g. `i64`, which
  would behave the same here.
- **Nonzero C results become a `()?` failure, and `main` uses `.must()`.** Other choices: drop them
  with `_ =`, which hides mutex errors, or test them in an `assert`, which puts a side effect inside
  an assertion.
- **`link "pthread"`.** The `counter_*` functions are inline in the header, but they call
  `pthread_mutex_*`. Other choice: no `link`. That works where libc provides pthreads (glibc 2.34+,
  macOS), but fails to link on older systems.
- **Loops are `while` with counter cells.** By the grammar, `for` needs a real `ident`, and `_` is a
  separate token, so `for _ in range(...)` would not parse. `for i in ...` would leave `i` unused,
  which is an error. Other choice: write out five increment calls.
- **Events are counted in `main`, next to each increment.** Other choice: a helper taking `@s: Stats`
  that does both.
- **No `Stats` comparisons or map keys.** It holds a `partial` record, so either would be a compile
  error.

context: yes. Besides the folder's files, I got the agent environment prompt: identity as a Claude
agent, working directory, date (2026-10-16), and tool-use instructions. I had only a read tool, so I
could not write files and gave their contents in my reply. No other rules, memory or repository
status reached me.

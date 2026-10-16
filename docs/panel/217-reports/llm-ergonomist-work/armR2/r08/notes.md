# Notes

- **How `doubled` reaches the counter.** The compiler forbids a by-value copy because of the
  `pthread_mutex_t`. I followed its suggested fix: `doubled(@c: Counter)`, called as
  `doubled(@s.first)`. The `@` only lends the place; nothing writes to it, so the counter is
  unchanged as the task requires. Passing `@c` on to `counter_value` counts as a use of the `@`
  parameter, so it is not "an `@` parameter nothing writes". Other choices: read `c.n` directly
  (the note allows "a field is read in place"), but the task says to use `counter_value`; or
  declare `counter_value(c: Counter lent)` by value, which is the same refused copy.
- **`counter_value` is declared `@c: Counter lent`** even though C takes `const struct counter *`.
  The spec has no const place. A `ptr` parameter would lose the type and need `.ptr()`, which only
  lends fields.
- **`Counter` is `partial` with only `n`.** The lock is `pthread_mutex_t`, which has no field type
  here, so it is left to `partial` and the record stays C's size. Naming the lock would need a
  record for pthread's struct, which is platform-specific.
- **`Counter(rest: zero)` before `counter_init`.** Every value must be initialised; `counter_init`
  then sets `n` and the lock in place. Leaving out `rest: zero` is not an option: it is the only
  way to build a record that has no fields you can name.
- **Errors:** the helpers return `()?`, and `main` (which returns nothing) uses `.must()`, so a
  failing C call aborts the program with its code. `?` is not allowed in `main`; `.default` on
  `()?` would hide the failure.
- **Loops:** `while` with counters. `for _ in range(from: 0, to: 3)` would work only if `_` counts
  as an `ident` in `For`, which the spec leaves unclear.
- **No `pthread_mutex_destroy`:** the header offers none, so the locks are never destroyed. This
  is harmless when the program exits.
- Expected output: `6`, `4`, `5`, one per line.

context: no. Only the files in this folder (brief.md, heroes-spec.md, task.md, counter.h,
previous.hero, compiler.txt) and the session's working directory and date reached me. No
instruction, rule, memory or repository status from outside the folder did.

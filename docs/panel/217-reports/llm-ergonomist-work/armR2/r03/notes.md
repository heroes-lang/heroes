# Notes

- **How `doubled` takes the counter.** The compiler refused 4 copies of a `Counter`, because its
  `struct counter` holds a `pthread_mutex_t`. I followed its suggested fix: `doubled(@c: Counter)`,
  called as `doubled(@s.first)`, with no local copy. The value stays the same because
  `counter_value` only reads it.
  The other choice, reading `c.n` directly as the note suggests, avoids the `@` but breaks the
  task's rule to use `counter_value`. Any version that takes the counter by value, or copies it,
  is refused again with `ffi_lock_copied`.
- **`counter_value`'s parameter.** C takes `const struct counter *`, so I kept `@c: Counter lent`.
  Heroes has no word for const. Without `lent`, the parameter counts as keeping what it is given,
  so a lend could not reach it. By value it would be a refused copy.
- **Does `doubled` count as writing its `@` parameter?** The spec says an `@` argument counts, so
  passing `@c` to `counter_value` is enough. Otherwise `doubled` would be refused for an `@`
  parameter that nothing writes.
- **The record and how it is built.** I kept `partial` with only `n` (the mutex is not a field type
  Heroes can name) and `Counter(rest: zero)` before `counter_init`. The compiler did not complain
  about these lines. The other choice, a fixed byte array standing in for `lock`, depends on the
  platform's mutex layout.
- **C's return codes.** I `assert` that they are `== 0`, so a failed init, lock or unlock stops the
  program. Discarding them with `_ =` would hide those failures.
- **Loops.** I used `while` with counters. Writing `for i in range(from: 0, to: 3)` would leave `i`
  unused, and the spec does not say whether a `for` may bind `_`.

context: yes. A system prompt reached me: it said I am a Claude agent built on the Claude Agent SDK,
gave the working directory, gave the date 2026-10-16, and explained how to call tools. It had no
rules about Heroes, no memory and no repository status. It gave me a read-only tool, so I could not
write these files myself.

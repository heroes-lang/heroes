# Notes

- **`pthread_mutex_t lock`**: the spec gives no field type for it, so I declared `Counter`
  as `partial` with only `n`. The size stays C's. The other choice was to declare `lock`
  through its own group record, which would mean copying a platform-specific struct layout.
- **`tag counter`**: the header says `struct counter`, so I used `tag`. A record called
  `counter` with no tag goes against "same name unless … after the word struct".
- **Construction**: `Counter(n: 0, rest: zero)` names `n` and zeroes the lock. Writing
  `Counter(rest: zero)` and naming nothing is only clearly allowed for unions, so I avoided it.
- **Setting up a counter**: done in place through an `@` parameter (`setup(@s.first)`), not
  returned as a value. Returning it would move an initialised mutex. The spec allows that
  because C keeps no pointer, but POSIX says a copied mutex is undefined.
- **Pointer parameters**: `@c: Counter lent` for `struct counter *`, so C gets the place
  itself and doesn't keep it. `c: Counter lent` without `@` for `const struct counter *`,
  which is a pointer to const. Dropping `lent` would mean C keeps what it's handed, and the
  lend would be refused.
- **`int` results**: declared `i32`, C's own width. `i64` was also allowed because a result
  may be wider.
- **Error results**: I checked them and passed them up as `()?` failures. `main` prints the
  error and exits with 1. The other choices were `_ = counter_lock(@c)`, which ignores
  failures, or `assert`, which aborts.
- **Loops**: I used counted `while` loops. The `For` grammar takes an `ident`, and the spec
  doesn't say `_` counts as one, while a named loop variable that's never used is an error.
  Writing out the five calls by hand would also have worked.
- **Linking**: no `link "pthread"`. On macOS and modern glibc, pthread is part of libc.
  Older glibc would need it.
- **Two `@` places from one value**: I avoided helpers like `bump(@s.first, @s.events)`. The
  spec says no other `@` argument may reach the place, but doesn't say whether different
  fields of one cell count as separate places.

context: yes. The system prompt said I'm a Claude agent and gave the working directory and
the date (2026-10-16). It also only gave me a file-reading tool, so I couldn't write these
files myself. There was no repository status, memory or other rule.

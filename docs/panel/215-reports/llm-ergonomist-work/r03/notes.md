# Notes

## All three programs
- C takes a `const struct *`. I pass the record as `@p: T lent` on a `@=` cell, because a
  C out-parameter "receives the place itself". Without `@` the struct would be passed by
  value, which doesn't match the header. `lent` because no function keeps the pointer;
  without it the spec assumes C keeps it, and lending a local would be refused.
- A C `int` result is declared `-> i64`, since a result may be wider than C's. `-> i32`
  would work too.
- There's no `link`: every function is `static inline` in its header.

## a1.hero
- Building `path`: `rest: zero` zeroes it, then a loop writes the 16 bytes of the text.
  The other way is a 108-element `[...]` literal of character literals and zeros. That
  gives the same bytes, but it's long and easy to get wrong.
- `char` is `i8`, as the spec says, and each byte is converted with `to_i8(..).must()`.
  ASCII can't fail. Declaring `u8[108]` would be refused, because a field must match the
  header's sign.
- Expected output: `16`.

## a2.hero
- How to declare a named union (`via`): I list its members `fd` and `handle` directly in
  the record, reading "a field in a C union, named or anonymous". The other option was
  `partial` with only `kind`. That depends on `rest: zero` also zeroing bytes the record
  doesn't declare, which the spec doesn't say.
- I declare both union members, though one would be enough ("one or more of each
  union").
- The record is built naming no union member plus `rest: zero`, so the whole union is
  zero. Naming `fd: 0` would zero only 4 of its 8 bytes.
- Expected output: `2`.

## b.hero
- `pthread_mutex_t` can't be a field type, so the record is `partial` with only `n`.
  Size stays C's.
- `bump(@c: Counter)` is copy-in/copy-out, so the mutex is copied to a new place on each
  call while it is unlocked. POSIX leaves using a copied mutex undefined. I can't avoid
  this without memory owned by C, and this header gives none. Doing the work inline in
  `main` would avoid the copy but breaks the "write a function" requirement.
- Return codes are checked by a `check` helper that calls `exit(code: 1)`. The other
  choices were `_ = counter_lock(@c)`, which ignores failure, or `assert rc == 0`. It
  isn't clear whether `assert` runs outside tests.
- `n` is incremented in Heroes while the lock is held, not in C.
- Expected output: `3`.

context: yes. The agent's system prompt reached me: it described me as a Claude Agent
SDK agent and gave the working directory, the date (2026-10-14) and tool-use rules. The
user's message pointed me to brief.md. I received no memories and no repository status.
I only had a Read tool, so these files were returned as text rather than written to disk.

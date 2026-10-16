# Notes

- **Setting up a counter.** The task says "sets up a new counter for its caller". I made it
  `setup_counter(@c: Counter) -> ()?`, which runs `counter_init` on the caller's own place.
  Returning a new `Counter` (`-> Counter?`, as before) copies it out (`ok(c)`, then `.must()`
  into `Stats`), and the compiler refused both copies.
- **Building `Stats`.** The fields are filled with `Counter(n: 0, rest: zero)` and initialised in
  place afterwards. The spec doesn't say whether putting a fresh construction into a field counts
  as a copy. The compiler didn't flag `c @= Counter(n: 0, rest: zero)` (previous line 16), so I
  assumed it doesn't. The only other way was the refused `new_counter().must()`.
- **`doubled`.** The note's fix says to take `@c`. Taking it by value is a refused copy. Passing it
  on to `counter_value(@c)` counts as the write that an `@` parameter needs (spec section 5).
  `counter_value` only reads, so the counter is unchanged.
- **`lent` on the extern parameters.** The header's functions don't keep the pointer, so I kept
  `lent`. Without it, the compiler would assume C keeps the place, and a field place might then
  be refused.
- **`partial`.** `pthread_mutex_t` isn't a field type the spec allows, so `lock` is left out
  through `partial`. The C size is kept.
- **Result width.** C returns `int` and `int64_t`. I declared both as `i64`, since a result may be
  wider than C's. `i32` would also be accepted.
- **Errors.** A non-zero return from init, lock or unlock becomes a `fail`, and `main` stops on it
  with `.must()`. Ignoring the return codes would mean a failed lock goes unnoticed.
- **Loops.** I used `while` with counters, because `for k in range(...)` with `k` unused is a
  compile error. It's unclear whether `for _ in` is allowed, since the grammar says `ident`.

context: yes. The system prompt said I'm a Claude agent on the Claude Agent SDK and gave a working
directory and the date (2026-10-16). Only a file-reading tool was available, so I couldn't write
the files myself. No repository status, memory or other rules reached me.

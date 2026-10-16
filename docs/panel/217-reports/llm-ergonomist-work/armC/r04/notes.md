# Notes

- **Declaring `struct counter`:** I used `record Counter tag counter partial` with only the field `n: i64`. The
  language has no type for `pthread_mutex_t`, so `partial` leaves `lock` out and the record keeps C's size.
  The other choice was to name the record `counter` with no `tag`. That works but breaks the CapitalCase naming
  used for records.
- **Setting up a new counter:** `setup_counter(@c: Counter)` initialises the caller's own place. The other choice
  was a function that returns a `Counter` built and initialised inside it. Returning would move an initialised
  `pthread_mutex_t`, which POSIX doesn't allow (the spec also says a return moves a place). Because of my choice,
  `Stats` is first built with `Counter(n: 0, rest: zero)` and then initialised in place.
- **`lent` on the C parameters:** I marked all four as `lent`, because none of the functions keeps the pointer.
  Without it, each call would be taken to keep the place, and the program would have to stop it from moving.
  `counter_value` has no `@` because C declares it `const struct counter *`, so it is a pointer to const.
- **Result width:** `int` results are `i32`. The spec allows a wider result, so `i64` would also be accepted.
- **Return codes:** `check` prints a message and calls `exit(code: 1)` when a code isn't 0. Other choices were
  `assert`, which the spec doesn't clearly say runs in ordinary builds, or a `()?` result. A `()?` would make
  `main` call `.must()` on every step. Discarding the code with `_ =` would hide failures.
- **`link "pthread"`:** I added it. glibc 2.34+ and macOS don't need it but accept it. Leaving it out would fail
  to link on an older glibc.
- **Loops:** I used `while` with counting cells. `for _ in range(...)` probably doesn't parse, because `For`
  takes an `ident` and the grammar treats `_` as its own token. `for k in ...` would leave `k` unused, which is
  a compile error. Writing out five separate calls would also work.
- **Counting events:** `main` updates `s.events` directly, next to each `increment`. A helper taking both
  `@s.first` and `@s.events` would hand one call two `@` arguments inside the same `s`. The spec may forbid that
  ("no other `@` argument of the call may reach").
- **Passing fields:** the program passes `@s.first` and reads `s.first` as places inside the cell `s`. I'm
  assuming "C writes back only where the binding is a `@` name" covers fields of a `@=` cell and `@` parameters.
- **No mutex destroy:** `counter.h` doesn't provide one, so the mutexes are never destroyed.

context: yes. My setup said I am a Claude agent built on the Claude Agent SDK, gave a working directory and the
date 2026-10-16, and offered only a Read tool. That is why the files are in my reply rather than written here. No
other instruction, rule, memory or repository status reached me.

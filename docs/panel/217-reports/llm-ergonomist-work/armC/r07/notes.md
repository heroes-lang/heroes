# Notes

- **`partial` record.** `pthread_mutex_t lock` can't be a field (it isn't a number, bool, ptr, cstr or record of the group), so `Counter` is `tag counter partial` and declares only `n`. Its size stays C's. Declaring `lock` would be refused.
- **Setting up a counter.** `setup(@c: Counter)` initialises the caller's own place. Returning a new `Counter` instead would move the struct after `pthread_mutex_init`: a return moves a place, and copying an initialised mutex is undefined in POSIX.
- **Building before `counter_init`.** I wrote `Counter(n: 0, rest: zero)` so every byte, mutex included, starts at zero. Without `rest: zero`, the spec doesn't say what the left-out bytes hold. `Counter(rest: zero)` alone might also be allowed for a record without unions, but the spec only clearly allows naming nothing for unions.
- **`lent` on the C parameters.** C doesn't keep the pointer, so all four are declared `lent`. Without it, the spec treats C as keeping the pointer, and a field place (`@s.first` → `@c`) probably couldn't be lent.
- **`@` versus const.** The non-const `struct counter *` is `@c: Counter lent`. `counter_value`'s const pointer is `c: Counter lent` with no `@`, which the spec calls a pointer to const. Using `@` there would claim C may write.
- **Result width.** C's `int` results are `i32`. A wider `i64` is also allowed; it would only change the comparison type.
- **Library.** I added `link "pthread"`, which is safe on Linux and macOS. Leaving it out works on glibc 2.34+ and macOS, but fails on older glibc.
- **Failures.** A nonzero result from init, lock or unlock becomes a `()?` error, and `main` (which can't propagate) uses `.must()`, so it aborts. The alternative, `assert counter_lock(@c) == 0`, also aborts but gives the caller no error to handle. Ignoring the results with `_ =` is allowed for an `i32` but would hide lock failures. If unlock fails after the increment, the increment is kept and the error is reported.
- **Loops.** I used `while` with a counter cell. `for _ in range(from: 0, to: 3)` would be shorter, but the `For` rule asks for an `ident`, and it's unclear whether `_` counts.
- **Where events are counted.** In `main`, next to each call, which is what the task describes. Passing the whole `Stats` to the increment function would tie the function to `Stats`.
- **No mutex destroy.** The header has none, and nothing here is a handle or a lease, so nothing is owed at exit.

context: yes. My setup said I'm a Claude agent and gave the working directory and date (2026-10-16). It also said to make independent tool calls together. The folder path suggests an experiment ("llm-ergonomist/armC/r07"). I had a read-only tool, so I couldn't save these files myself. Nothing else reached me: no other rules, memory or repository status.

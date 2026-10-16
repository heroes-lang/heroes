# Notes

- **`struct counter` holds a `pthread_mutex_t`, which Heroes can't express.** Declared
  `record Counter tag counter partial` with only `n: i64`, so the size stays C's. Other choice:
  listing every field, which is impossible here.
- **Setting up "a new counter for its caller".** Done in place: `setup_counter(@c: Counter)`
  writes a fresh value into the caller's own place and calls `counter_init(@c)` there. Other
  choice: `new_counter() -> Counter?` returning an initialised counter. That moves the
  initialised mutex, which POSIX leaves undefined and the spec says C may not rely on.
- **Making an empty `Counter`.** Used `Counter(n: 0, rest: zero)`, so the left-out mutex bytes
  are zero. Other choice: naming only `n`, which the spec doesn't clearly allow for a `partial`
  record.
- **Pointer parameters.** `counter_init`, `counter_lock` and `counter_unlock` take
  `@c: Counter lent`: C writes but doesn't keep the pointer. `counter_value` takes
  `c: Counter lent`, which the spec says is a pointer to const. Other choice: leaving out `lent`,
  which would mean C keeps the pointer and would refuse lending a field place.
- **Result width.** C's `int` is declared `i32`. Other choice: `i64`, which the spec also allows
  ("a result may be wider").
- **Failure handling.** Non-zero returns from init, lock and unlock become `fail(...)` in a `()?`,
  and `main` calls `.must()`. Other choices: `assert rc == 0`, or ignoring the results, which the
  "must use" rule refuses.
- **Mutating a counter inside `Stats`.** Passed as `@stats.first`, so the mutex stays inside the
  `@` cell `stats`. Other choice: copying the counter out and back, which would move the mutex.
- **No `link`.** The functions are `static inline`, and pthread is in libc on current systems.
  Other choice: `link "pthread"`, which is harmless where it exists.
- **Loops.** Used `while` with an index cell. Other choice: `for i in range(...)`, where the
  unused `i` is an error and the spec doesn't say a `for` may bind `_`.
- **Doubling.** `counter_value(c) * 2` stops the program if it overflows. Values here are small.

context: yes — a system prompt said I am a Claude agent built on the Claude Agent SDK, gave the
working directory and date (2026-10-16), and gave tool-calling rules. Only a file-reading tool was
available, so these files were returned as text instead of written to disk. No memory or
repository status reached me.

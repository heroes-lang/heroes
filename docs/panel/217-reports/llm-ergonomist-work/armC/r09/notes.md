# Notes

- **Record name / tag**: `record Counter tag counter partial`. The header names the type after the word
  `struct`, so `tag` is needed. Another choice: call it `counter` with the same tag; this changes only the name.
- **`partial`**: `pthread_mutex_t lock` is not a field type the language allows, so I left it out and
  named only `n`. The record keeps C's size. Another choice: none, because a full field list cannot
  declare the mutex. One side effect: `Counter` and `Stats` cannot be compared or used as map keys.
- **Setup returns nothing; it initialises in place** (`setup_counter(@c: Counter) -> ()?`). The spec
  says a return or a copy moves a value, and POSIX does not allow copying a mutex after it is set up.
  Another choice: `new_counter() -> Counter?`. It reads more like "gives the caller a new counter", but
  it copies a set-up mutex once when returning and again when building `Stats`.
- **Building `Stats` first**: `Counter(n: 0, rest: zero)` gives the fields a value before `counter_init` runs.
- **`lent` on every C parameter**: C does not keep the pointer after the call. Without `lent`, C is
  taken to keep it, and a field of `s` could not be lent to it.
- **`@` vs pointer to const**: `counter_init`, `counter_lock` and `counter_unlock` take `@c` (C may
  write). `counter_value` takes `c: Counter lent` without `@` (`const struct counter *`).
- **Result widths**: `int` is declared as `i32`. Another choice: `i64`, since a result may be wider than C's.
- **Errors**: a nonzero result from `counter_init`, `counter_lock` or `counter_unlock` becomes `fail(...)`,
  and `main` calls `.must()` on it, so the program stops. Another choice: ignore the codes; that would
  need `_ = ...` and would hide failures.
- **Who changes `n`**: Heroes does `c.n @ c.n + 1` between lock and unlock. The header has no C function
  for incrementing.
- **Loops**: I used `while` with counter cells. `for _ in range(...)` may not parse, because `For` takes
  an `ident`, and a named loop variable that is never read is an "unused" error.
- **`link "pthread"`**: added for older glibc. On newer libc it is redundant but harmless. Another choice:
  leave it out, which may fail to link on old systems.
- **No `pthread_mutex_destroy`**: the header offers none, so the mutexes are never destroyed. They live
  until the program exits.

context: yes. Besides the files, I received a system prompt: I am a Claude agent built on the Claude
Agent SDK, the working-directory path, the date (2026-10-16), and instructions on how to call tools.
I also received the user's message telling me to follow brief.md. No memory or repository status reached me.

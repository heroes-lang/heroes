# Notes

- **The struct**: declared `record Counter tag counter partial` with only `n: i64`, because
  `pthread_mutex_t` can't be written as a field. Its size stays C's. Other choice: listing the
  lock is impossible; leaving out `partial` would be refused.
- **Building it**: `Counter(n: 0, rest: zero)` zeroes the mutex bytes before `counter_init`.
  Other choice: without `rest: zero`, a partial record can't be fully built.
- **Setting up a counter**: `setup(@c: Counter) -> ()?` initialises the caller's own place, so
  the mutex is never copied or moved after init (the spec says a copy or return moves a place).
  Other choice: `new_counter() -> Counter?` returning an initialised counter would copy an
  initialised mutex. POSIX leaves that undefined, though it usually works.
- **Results**: C's `int` results are `i32`. Other choice: `i64` is allowed, since a result may
  be wider; same behaviour.
- **Return codes**: a non-zero init/lock/unlock code becomes `fail(...)`, and `main` calls
  `.must()`. Other choice: `_ = counter_lock(@c)` ignores failures silently, and an unlock
  failure would go unseen.
- **`counter_value`**: declared `c: Counter lent` (pointer to const), so `doubled` can't change
  the counter. Other choice: `@c` would need a writable place and would allow C to write.
- **No `link "pthread"`**: modern libc includes the pthread functions. Other choice: adding it
  is harmless where `libpthread` exists and would be needed on old glibc.
- **Loops**: counted `while` loops with cells. Other choice: `for _ in range(...)` depends on
  whether `_` counts as an `ident` in `For`, which the grammar doesn't make clear; `for i in`
  would make `i` an unused-binding error.
- **Event counting**: done in `main` next to each increment. Other choice: an `@s: Stats`
  helper would keep the counter and event count updated together but mixes the two jobs.
- **No mutex destroy**: the header doesn't provide one, and nothing is owed at exit.

context: yes — besides the files I received a system prompt: I'm a Claude agent on the Claude
Agent SDK, with a working directory (whose path includes `217-llm-ergonomist/armF1/r10`), a date
(2026-10-16), and rules for calling tools. My only tool reads files, so I couldn't write the
output files and gave their contents in my reply instead. No repository status or memory came
with it.

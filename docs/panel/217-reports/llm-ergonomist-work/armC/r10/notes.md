# Notes

- **The `lock` field (`pthread_mutex_t`)**: it is an opaque struct, which is not a field type
  the spec allows, so I left it out with `partial`. A `Counter` is built with
  `Counter(n: 0, rest: zero)`. The other choice, declaring the mutex as a record of the group,
  would mean copying platform-specific internals and the header check would probably refuse it.
- **Record name**: `record Counter tag counter`. Calling the record `counter` with no `tag`
  would also match the header, but breaks the naming used elsewhere in the spec.
- **"Sets up a new counter for its caller"**: done in place, `setup_counter(@c: Counter) -> ()?`.
  Returning a `Counter` from a function would move an initialised mutex, which the spec says C
  may not rely on and POSIX leaves undefined.
- **`lent` on the `@` C parameters**: C does not keep the pointer after the call. Without
  `lent`, C is taken to keep it, so the place would have to stay put for good.
- **`counter_value` takes `c: Counter lent`**: this is how the spec writes a pointer to const.
- **C return types**: `int` is declared `i32`. A wider `i64` is also allowed for a result.
- **A non-zero return from init, lock or unlock**: it becomes a `T?` error, and `main` calls
  `.must()`, so the program aborts with a message. Ignoring it is not allowed: the value has to
  be used.
- **`link "pthread"`**: added so older glibc can find `pthread_mutex_*`. On modern glibc and
  macOS it is harmless. Leaving it out would fail to link on old glibc.
- **Loops**: I used `while` with a counter cell, not `for _ in range(...)`. The grammar has
  `for ident`, and `_` does not seem to be an `ident`, while a named loop variable that is never
  read would be an "unused binding" error. Writing the five calls out by hand would also work.
- **Error handling in `main`**: `.must()` on each call. The other choice is a `run() -> ()?`
  helper using `?`, with `main` matching on its result to print the error code.

context: yes. The environment description said I was a Claude agent built on the Claude Agent
SDK, and gave the working directory and date (2026-10-16). There was no repository status and
no memory. I only had a file-reading tool, so I could not write these files myself.

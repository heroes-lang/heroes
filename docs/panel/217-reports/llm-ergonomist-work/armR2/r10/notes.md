# Notes

- **How `doubled` gets its counter.** The compiler forbids copying a `Counter`, so
  the choice was between taking `@c` (its suggested fix) and having the caller read
  `s.first.n` directly. I took `@c` and pass it straight to `counter_value(@c)`,
  with no local copy. Reading the field directly would avoid `@` but would skip
  `counter_value`, which the task requires. Because of `@`, the signature doesn't
  promise the counter stays unchanged; the function body is what keeps that
  promise.
- **Whether `doubled`'s `@c` counts as used.** The spec says an `@` parameter
  nothing writes is an error, but an `@` argument counts as a write. Passing
  `@c` to `counter_value` is such an argument, so the parameter is valid. If the
  function read `c.n` instead, it would be an unwritten `@` parameter, which is an
  error.
- **How `counter_value` is declared.** C takes `const struct counter *`. I kept
  `@c: Counter lent`. Declaring it by value would copy the lock and be refused.
  Without `lent`, the spec says C is taken to keep the pointer.
- **Locking in `doubled`.** The task asks for the lock only around the increment,
  so `doubled` reads without locking. Adding lock and unlock calls would make it
  fallible (`-> i64?`) for no gain, since the program has one thread.
- **Loops.** I used counted `while` loops. `for k in range(...)` would leave `k`
  unused, which is an error, and the grammar doesn't clearly allow `_` as a `for`
  variable.
- **Linking.** Like before, there is no `link "pthread"`, since pthread is part of
  libc on current systems and the compiler didn't object. If the target needs it
  separately, add `link "pthread"`.
- **Failures.** The C return codes become `T?`, and `main` aborts with `.must()`.
  Using `.default` or ignoring them would hide a failed lock.

context: yes. Besides the files here, I received a system prompt saying I'm a
Claude agent on the Claude Agent SDK, with the working directory, the date
(2026-10-16) and tool-use rules. I also received your message telling me to
follow brief.md. Only a file-reading tool was available, so I couldn't write
these files myself. No memory, repository status or other rules reached me.

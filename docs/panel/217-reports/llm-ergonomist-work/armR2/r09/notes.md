# Notes

- **How `doubled` gets the counter.** The compiler refused copying a `Counter`
  (its struct holds a `pthread_mutex_t`), so `doubled` now takes `@c` and each
  call passes `@s.first` / `@s.second`, as the compiler's fix suggested.
  The other options: reading `c.n` directly needs no `@`, but the task says to
  use `counter_value`; any by-value parameter or local copy brings back
  `ffi_lock_copied`.
- **"Without changing the counter".** Taking `@c` means `doubled` could write the
  counter, but it only passes it on to `counter_value`, which reads it (`const`
  in C). That pass-on counts as the write the spec requires of an `@`
  parameter, so `@c` is not flagged as unused. If `doubled` passed `@c` to
  nothing, it would be a compile error.
- **Removed `view @= c`.** It was a copy, and with `@c` there is nothing to copy.
- **Everything else is unchanged from previous.hero.** The compiler reported
  nothing else. This includes building the counters with `Counter(rest: zero)`
  inside `Stats`, rather than with a separate setup function that returns a
  `Counter`. Returning one would move it and might count as a copy. Failures
  still stop the program with `.must()`; propagating them with `?` is not
  possible because `main` returns nothing.
- **Result width.** `doubled` returns `i64` to match `counter_value`. If the value
  overflowed it would abort, but it can't here.

Expected output: 6, 4, 5, one per line.

context: yes. The environment told me the working directory, the date
(2026-10-16), that I am a Claude agent, and how to call tools. I had no tool
that writes files, so this answer gives the file contents instead of saving
them. Nothing else reached me: no other rules, memories or repository status.

# Notes

- **`struct counter` as `record Counter tag counter partial`, naming only `n`.** A
  `pthread_mutex_t` is not a type a field can have (not a number, bool, ptr, cstr, or a record
  or fixed array of the group), so `partial` leaves it out and the size stays C's. Naming `lock`
  would be refused. A field-less `tag counter` would be a *handle* (`struct counter *`), which
  needs C to allocate one, and nothing here does that.
- **Setup initialises in place (`@c: Counter`) instead of returning a new `Counter`.** C gets
  the place itself, and a return moves it. Returning would copy a mutex after
  `pthread_mutex_init`, which POSIX leaves undefined. The caller therefore builds the record
  first with `Counter(n: 0, rest: zero)` (naming `n`; `Counter(rest: zero)` would be the same).
- **The C `@` parameters are marked `lent`.** None of the inline functions keep the pointer.
  Without `lent`, C is taken to keep the place, so `Stats` could never move, and lends could
  not reach those parameters.
- **`counter_value(c: Counter lent)` without `@`.** This is the header's `const struct counter *`.
  Using `@c` would need a mutable place and would claim that C writes to it.
- **Result widths: `i32` for `int` and `i64` for `int64_t`, the exact widths.** Wider types
  (for example `i64` for `int`) were also allowed.
- **Nonzero return codes become `fail(...)`, and `main` reports the error and calls `exit(code: 1)`.**
  The other options were discarding with `_ =` (allowed, since the results are not fallible),
  `assert rc == 0`, or `run().must()` (which aborts with the runtime's message).
- **`doubled` reads without taking the lock.** The task asks for a lock only around the
  increment. Locking would mean `@c` and an `@` argument at the call site.
- **No `link "pthread"`.** The functions are `static inline`, and pthread is in libc/libSystem on
  current systems. An old glibc would need `link "pthread"`.
- **Loops are written `for _ in range(from: 0, to: 3)`.** I read "`_` names anything you do not
  use" as covering a loop variable. The other choice is a `while` loop with a counter cell, or
  writing the calls out by hand.
- **The `Stats` literal is split across lines inside parentheses.** Writing it on one line is
  equivalent.

context: yes — besides the folder's files, a harness system prompt reached me (it said I am a
Claude agent, gave the working directory and date, and gave tool-calling rules), plus the
user's instruction to follow brief.md. No memory, rules file or repository status reached me.
I had only a file-reading tool, so I could not write these files myself.

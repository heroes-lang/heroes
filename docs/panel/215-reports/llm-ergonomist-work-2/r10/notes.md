# Notes

## a2.hero
- `via` is an anonymous union with no typedef, so no record can type it. I left it to
  `partial` and zeroed it with `rest: zero`. Naming it would need a union typedef the header
  doesn't have.
- The record is called `Tagged` with `tag tagged`, because the header writes the name after
  `struct`. Calling it `tagged` with no tag would not match.
- `const struct tagged *` is an `@ ... lent` parameter, so the record is lent whole. Without
  `@` the struct would be passed by value, against a pointer parameter. Without `lent`, C
  would be taken to keep the address.
- The result is `i32` (C's width). `i64` would also be allowed ("a result may be wider").

## u.hero
- I used the five 256-byte fields that tasks.md gives. A real Linux header has 65-byte
  fields and a `domainname` field, and clang would refuse this declaration there.
- `char` → `i8[256]`, as section 13 says. `u8` would disagree in sign and be refused.
- The struct is built empty with `Utsname(rest: zero)`, not with five 256-element literals.
- If `uname` fails, or `sysname` is not valid text, the program prints a message or code and
  exits 1. The other choice was `.must()`, which aborts.

## c.hero
- `pthread_mutex_t` can't be a field of the group, so `Counter` is `partial` with only `n`.
  Its size stays C's.
- "Sets up a new counter" initialises in the caller's place (`@c`), so the mutex is never
  copied after init. The other choice, returning a `Counter`, would move (copy) an
  initialised mutex, which POSIX leaves undefined.
- `doubled` takes `c: Counter` by value, so the signature itself promises no change. It copies
  into a local cell, because `counter_value` needs a pointer (`@`). Taking `@c` would avoid
  the copy, but the signature would then allow writes.
- Non-zero C results become `fail(...)`, and `main` uses `.must()`, which aborts. The other
  choice was to ignore them with `_ =`.
- I used `while` counters instead of `for _ in range(...)`, because the grammar's `for`
  takes an `ident` and I don't think `_` counts as one. A named loop variable would be an
  unused binding.
- `events` is counted in `main` after each successful increment. Counting it inside
  `increment` would mean passing the whole `Stats`.

context: yes. Besides the files in this folder I received the harness's system prompt: it
said I am a Claude agent built on the Claude Agent SDK, and gave the working directory, the
date (2026-10-14) and a tool list. That list had only a file-reading tool, so I could not
write the files. No repository status, memory or other rule reached me.

# Notes

## a2.hero
- `via` is a union with no typedef, so the spec gives it no record type. I left it to `partial`, and
  `rest: zero` zeroes it. The other choice, a record for the union, isn't possible without a typedef.
- I used `tag tagged` because the header only names the struct after the word `struct`. Without the tag,
  clang would look for a type called `Tagged` and refuse it.
- `tg_kind` takes a `const struct tagged *`, so I passed the whole record with `@` from a local cell and
  marked it `lent`. Passing it by value would declare a struct parameter where C has a pointer, and clang
  would refuse it. Without `lent`, C would be taken to keep the address of a local.
- The result is `i32`, as in C. `i64` is also allowed (a result may be wider) and prints the same.

## u.hero
- The record is `partial` with only `sysname`. Listing all five fields would match the brief's header,
  but clang would refuse it on systems whose real header has more fields (Linux adds `domainname`).
- `char` is `i8[256]`. I read the field with `validated_bytes()`, up to the first zero.
- A failed `uname` prints a message and exits with 1. A text error prints its `msg`. Using `.must()`
  instead would abort.

## c.hero
- `pthread_mutex_t` isn't a record of the group, so `Counter` is `partial` with only `n`.
- `setup` takes the caller's place (`@c`) and doesn't return a new counter. Returning would move the
  initialised mutex, and POSIX says a copied mutex is undefined.
- `doubled` takes the counter by value and calls `counter_value` on a local copy, so the caller's counter
  can't change. A `doubled(@c)` signature would also compile, but it suggests the function changes the
  counter.
- Return codes are checked with `assert ... == 0`. `_ = ...` would silently ignore a failure.
- I wrote the repeats as `while` loops. `for i in range(...)` leaves `i` unused, which is an error, and
  the spec doesn't say whether `for _` is allowed.
- `increment` changes `c.n` directly while the lock is held, because the header has no increment function.

context: yes. The system prompt said I'm a Claude agent built on the Claude Agent SDK, and gave the
working directory (a git worktree path, `heroes-lang/.claude/worktrees/readers2/r03`) and the date
(2026-10-14). My only tool was Read, so I couldn't write files. No other instructions, rules, memories
or repository status reached me.

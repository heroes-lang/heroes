# Notes

## a2.hero
- `via` is an unnamed union with no typedef, so no record can type it. I left it out with `partial`, and
  `rest: zero` zeroes it. Naming the union field instead is not possible without a typedef.
- The header writes `struct tagged`, so I used `tag tagged` with my own name `Tagged`. Calling the record
  `tagged` without `tag` would, as I read the spec, be refused.
- C takes `const struct tagged *`, so the parameter is `@t ... lent` and `t` is a cell. A plain `t: Tagged`
  would pass the struct by value, which the header check would refuse. Dropping `lent` would mean C keeps
  the address.
- The result is `i32`, C's width. `i64` is also allowed ("a result may be wider") and would print the same.

## u.hero
- I declared all five fields at 256 bytes, as `tasks.md` shows. On a real Linux header (65-byte fields and
  a sixth field) clang would refuse this. `partial` with only `sysname` would get past the extra field, but
  not the size.
- `char` becomes `i8`, as the spec says. `Utsname(rest: zero)` names no field. If the spec doesn't allow
  that, there is no practical other way to build it.
- If `uname` fails I print a message and exit with code 1. Ignoring the result would print whatever is in
  the zeroed buffer (an empty line).
- `validated_bytes()` failing prints the error code. `.must()` would abort instead.

## c.hero
- `pthread_mutex_t` is not a type a group can declare, so `Counter` is `partial` with only `n`. Its size
  stays C's.
- "Sets up a new counter for its caller": I chose an `@` parameter that sets up the caller's own place, so
  the mutex is never copied after init. The other choice, `function new_counter() -> Counter`, returns
  the record and moves (copies) the initialised mutex. POSIX leaves that undefined, though it usually
  works.
- `doubled` takes `c: Counter` without `@`, so it cannot change the counter. It copies the counter into a
  local cell because `counter_value` needs a pointer. The other choice, `@c: Counter`, avoids the copy but
  lets the signature allow writes.
- Failed C calls return a `()?`, and `main` aborts on it with `.must()`. Ignoring the codes is not allowed:
  every value must be used.
- The loops are `while` loops with counting cells, because I don't know whether `for _ in range(...)` is
  allowed (the grammar wants an ident). Unrolling the five calls would work just as well.
- There is no `link`: pthread is in libc on current systems. Older glibc would need `link "pthread"`.
- All C parameters are `lent`, since none of the functions keep the pointer after the call.

## Tooling
- I could only read files, not write them, so these contents were given in my reply instead of saved.

context: yes. The system prompt reached me: it named me a Claude agent built on the Agent SDK, gave the
working directory (a `.claude/worktrees/readers2/r07` path), the date 2026-10-14, and rules for calling
tools. No memory, repository status or project rules reached me.

# Notes

## a2.hero
- Record name: the header only writes `struct tagged` (no typedef), so `record Tagged tag tagged`.
  Other choice: `record tagged` with no tag would not match the header's name and would be refused.
- `via` is an anonymous union with no typedef, so no record can type it. Left it out with `partial`,
  and `rest: zero` zeroes it. Other choice: naming `via` would need a union typedef record; there isn't one.
- `const struct tagged *` → `@t: Tagged lent` (a pointer to one place, not kept). Other choice: leaving out
  `lent` means C is taken to keep the place; the lend would then be refused.
- Result `i32` (C's int). Other choice: `i64` is allowed (a result may be wider) and prints the same 2.
- No `link`: the function is static inline in the header.

## u.hero
- Followed the struct as tasks.md gives it (5 fields of `char[256]` → `i8[256]`, since char is `i8`).
  Other choice: on glibc the real header has 65-byte fields and a domainname field. clang would refuse
  this declaration there, and it would need those widths and fields, or `partial` naming only `sysname`.
- Built with `Utsname(rest: zero)` and no field named, so I don't have to write five 256-element literals.
- `struct utsname *` out-parameter → `@buf: Utsname lent` on the `@` cell `u`, so C's writes come back.
- Text via `u.sysname.validated_bytes()` (reads up to the first zero). On `not_text` it prints the code.
  Other choice: `.must()` would abort instead.
- uname failure: print and `exit(code: 1)`. Other choice: `assert rc == 0`.

## c.hero
- `pthread_mutex_t` is not a number, record of the group or fixed array, so `Counter` is `partial`
  with only `n`; its size stays C's. Other choice: none portable (the mutex layout differs by platform).
- "Sets up a new counter for its caller": `setup(@c: Counter) -> ()?` initialises in the caller's place.
  Other choice: `new_counter() -> Counter?` returning one. That moves an initialised mutex (a copy),
  which POSIX leaves undefined, though it is accepted by Heroes since the lend is `lent`.
- Lock/init failures become `fail(...)` and main uses `.must()`. Other choice: `assert` in the callee, which aborts the same way.
- `doubled` takes the counter by value and lends a local copy `view` to `counter_value` (C needs a pointer,
  so `@`). This guarantees the caller's counter is untouched. Other choice: `doubled(@c: Counter)` avoids
  the copy, but its signature then allows mutation.
- Loops use counted `while` cells instead of `for _ in range(...)`. I wasn't sure `_` is accepted as
  the `for` variable, and a named unused one is an error.
- No `link "pthread"`: tasks.md says nothing else needs linking. Other choice: older glibc would need it.
- No `pthread_mutex_destroy` (the header doesn't name it); the mutexes are left alive at exit.

context: yes — the harness's system prompt reached me. It identifies me as a Claude agent, gives the
working directory (a path under `.claude/worktrees/readers2/r04`, suggesting a git worktree), the date
2026-10-14, and a tool list containing only a file reader. No other instructions, memories or
repository status.

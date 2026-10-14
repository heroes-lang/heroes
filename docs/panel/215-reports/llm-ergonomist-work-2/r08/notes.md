# Notes

## a2.hero
- `via` is a union with no typedef, so the spec gives it no record type. I left it to `partial`.
  It is zeroed by `rest: zero`. The other choice, declaring it as a field, would be refused:
  there is no group record to give it as a type.
- `const struct tagged *` is declared as `@t: Tagged lent`, so a cell `t @=` is needed. Passing
  it by value (`t: Tagged`) would not match a pointer parameter and would be refused.
- The result is `i32`, the header's own width. Declaring `i64` is also allowed (a result may be
  wider) and would print the same.

## u.hero
- `char[256]` is declared as `i8[256]`, the header's width and sign. Declaring `u8[256]` would be
  refused.
- I declared all five fields, not `partial` with only `sysname`. A partial version would behave
  the same but could not be compared.
- The struct is built with `Utsname(rest: zero)`, naming no field. The other choice is writing
  five 256-element array literals.
- A non-zero result from `uname` and a `not_text` failure both print a message and `exit(1)`.
  Using `.must()` instead would abort with the error.

## c.hero
- `pthread_mutex_t` is not a number, pointer or group record, so `Counter` is `partial` with only
  `n`. Declaring `lock` would be refused.
- The setup function fills the caller's place (`init_counter(@s.first)`) instead of returning a
  `Counter`. Returning one would move an initialised mutex, which POSIX does not allow (undefined
  behaviour).
- `doubled` takes the counter by value and copies it into a local cell for `counter_value(@local)`.
  That keeps the caller's counter unchanged, as the task asks. Taking `@c` would allow writes
  and mark the caller's argument `@`. The copy of the mutex is never locked.
- Errors from init, lock and unlock come back as `()?` and main calls `.must()` on them. The other
  choice, `assert rc == 0` inside the functions, aborts the same way but gives no error code.
- The loops use `while` with counter cells. `for _ in range(...)` may not be valid, since the
  `For` rule asks for an ident, and `for i in` with `i` unused is a compile error.
- All C functions are marked `lent` because none keeps the pointer. Without `lent`, C is taken to
  keep what it is handed.
- Output: 6, 4, 5.

context: yes. The agent's system prompt gave me the working directory path, today's date
(2026-10-14) and the description of my tools. My only tool reads files, so I could not write these
files myself. No other instruction, rule, memory or repository status reached me.

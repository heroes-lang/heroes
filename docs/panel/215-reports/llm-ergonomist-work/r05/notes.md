# Notes

## All three
- `const struct X *` parameters: declared `@p: X lent`. The spec only says an out-parameter is `@`;
  a plain record parameter would most likely mean the struct is passed by value, which does not
  match the header. Without `lent`, the spec treats C as keeping what it is handed.
- `int` results declared `-> i64` (a result may be wider than C's). With `-> i32` the program
  would print the same value.
- Record name: the header only writes `struct x`, so I used `tag x` with a CamelCase name. Using
  the C name directly, without `tag`, is the other reading of "the same name".

## A1
- `char path[108]` is `i8[108]` (spec: char → i8). `u8` would be refused.
- Built with `rest: zero` and the path left unnamed, then the 16 bytes written one by one in a
  loop (`to_i8().must()` on each byte). The other choice is to name `path:` with a 108-element
  literal, as the spec requires for a fixed array. Both give the same bytes; the output is 16.

## A2
- Union `via`: the record lists both members, `fd: i32` and `handle: ptr`, as plain fields. The
  spec says a record must name at least one of each union, so `fd` alone or `handle` alone would
  also do. It is unclear whether a named union's members need some kind of qualifier; I assumed not.
- Built with `kind: 2, rest: zero`, naming no union member, so the whole union and the padding
  are zero. `handle: nullptr` would also zero the union (it is as wide as the union) but says
  nothing about the padding. The output is 2.

## B
- `pthread_mutex_t` is not a field type the language can express, so `Counter` is `partial`,
  naming only `n: i64`.
- Construction is `Counter(n: 0, rest: zero)`. `Counter(rest: zero)` alone might also be accepted.
- `increment(@c: Counter)` is copy-in/copy-out: the mutex is copied in and back out ("moves"),
  and the language has no way to pass by reference. This works because the mutex is unlocked at
  every copy, but strictly speaking copying a pthread mutex is undefined. The alternative is to
  put lock / increment / unlock inline in `main`, which keeps the mutex in place but ignores the
  task's "write a function" requirement.
- Return codes of init, lock and unlock are checked with `assert … == 0`. The alternative,
  `_ = …`, ignores them silently. The output is 3.

context: yes — a system prompt told me I am a Claude agent built on the Claude Agent SDK, gave
the working directory (…/.claude/worktrees/readers/r05), the date (2026-10-14), and rules for
calling tools. Only a Read tool was available, so I could not write the files myself. No memory
or repository status reached me.

# Notes

## All three
- C `struct X` with no typedef: I used a Heroes name plus `tag X` (spec: "the same name unless
  the header writes it after the word struct, which `tag` gives"). Other choice: `record sock_path`
  etc. with no tag, which would name a C type `sock_path` that doesn't exist and would be refused.
- `const struct X *` parameter: declared `@p: X lent` and passed `@name`. Other choice: a non-`@`
  record parameter, which would pass the struct by value and not match the header. Without `lent`,
  the callee would be taken to keep the place.
- `int` result: declared `i32` (C's width). Other choice: `i64` (a result may be wider), with the
  same output.

## A1
- `char path[108]` is `i8[108]` (spec: "`i8` where it says char"). `u8[108]` would be refused.
- Filling `path`: I built the record with `rest: zero` and wrote the 16 bytes one at a time in a
  cell, converting each `u8` with `to_i8().must()`. Other choice: a 108-element array literal in the
  construction, which is legal but unwieldy. Both leave every other byte zero.
- Expected output: `16`.

## A2
- The union is named (`via`) and its type has no name, so no group record can stand for it. I
  listed its members `fd: i32` and `handle: ptr` as fields of `Tagged`, reading "a field in a C
  union, named or anonymous" to mean this. Other choice: `partial` with only `kind`. Then it is
  unclear whether `rest: zero` zeroes the bytes that aren't listed. If clang checks `fd` as
  `t->fd` rather than `t->via.fd`, my choice is refused.
- `rest: zero` and naming no union member: the spec allows naming "none when it ends with
  `rest: zero`", so `fd` and `handle` (no byte shared with `kind`) are zero.
- Expected output: `2`.

## B
- `pthread_mutex_t` is not a number, record or fixed array that a field can hold, so I used
  `partial` with `n` only. Other choice: none that the spec allows.
- Construction: `Counter(n: 0, rest: zero)`. Other choice: `Counter(rest: zero)`, the same bytes.
  `counter_init` sets `n` and the mutex anyway.
- `increment` takes `@c: Counter`, which copies in and copies out. The mutex bytes move across that
  copy while unlocked, and stay in place in the cell while locked. Other choice: inline the three
  calls in `main` to avoid the copy, but the task asks for a function.
- Return codes: I bound each one and `assert`ed `== 0`. Other choice: `_ = counter_lock(@c)`,
  which ignores failure silently.
- No `link`: the functions are `static inline`, and pthread is assumed to be in libc. Other choice:
  `link "pthread"` on older systems.
- Expected output: `3`.

context: yes. The environment gave me a system prompt saying I am a Claude agent, plus the
working directory path (a worktree `.claude/worktrees/readers/r07`) and the date 2026-10-14. It also
gave me only a file-reading tool, so I could not write these files myself. Nothing else (no
repository status, memory or extra rules) reached me.

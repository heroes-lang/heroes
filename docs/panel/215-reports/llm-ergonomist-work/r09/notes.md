# Notes

## All three
- Record name: the header names each struct only after the word `struct` (`struct sock_path`,
  `struct tagged`, `struct counter`), so I used `tag`. Leaving out `tag` would make clang look for
  a type called `sock_path` etc., which doesn't exist, so it would be refused.
- `const struct X *` parameter: declared `@p: X lent` and called with `@name`, so C gets the place
  itself and doesn't keep it. Passing it by value (no `@`) would hand C a struct, not a pointer,
  and the header check would refuse it. Leaving out `lent` means C is assumed to keep the pointer,
  so passing a lend to it is refused.
- `int` result: declared `i32`. `i64` is also allowed ("a result may be wider") and would print
  the same.

## A1
- `path`: I zeroed everything with `rest: zero`, then wrote the 16 bytes in a loop. The other way
  is a 108-element literal (a fixed array needs every element written): same bytes, but unreadable.
- Bytes: `to_i8(text[i]).must()`, since `char` is `i8` and `s[i]` is `u8`. The text is ASCII, so
  it never aborts.
- Expected output: `16`.

## A2
- Named union `via`: I listed its member `fd` flat on the record. The spec says union members are
  record fields but doesn't say how a *named* union is spelled. If it needs a `via` level instead,
  this is refused.
- I listed only `fd`, not `handle: ptr`. The rule is "one or more of each union"; listing both
  would also be legal.
- Built naming no union member, plus `rest: zero`, so all 8 union bytes are zero. Naming `fd: 0`
  instead would clearly zero only 4 of them; whether `rest: zero` covers the other 4 is unclear.
- Expected output: `2`.

## B
- `pthread_mutex_t` can't be written as a Heroes field, so `Counter` is `partial` and lists only
  `n`. Its size stays C's, so the mutex bytes travel with the value.
- Built with `Counter(n: 0, rest: zero)`. `Counter(rest: zero)` alone might also be allowed.
- `bump` takes `@c`, which is copy-in/copy-out. That moves a mutex, which POSIX doesn't promise
  works. It's harmless here because the mutex is always unlocked between calls and C keeps no
  pointer. The other way, a `ptr` handle to C-owned memory, is impossible here: the header has no
  allocator.
- Return codes: `bump` returns `()?` and fails on a nonzero lock or unlock result; `main` uses
  `.must()`, and checks `counter_init` with `exit(code: 1)`. The other options are dropping the
  codes with `_ =`, which I judged worse, or `assert`.
- Three written-out calls rather than a loop: `for _ in` may not parse, because `_` might not
  count as an identifier there.
- Expected output: `3`.

context: yes — besides the folder's files I received the agent system prompt: that I'm a Claude
agent, the working directory (a worktree path `.../readers/r09`) and the date 2026-10-14. No other
instructions, memories, or repository status. My only tool was a file reader, so I couldn't write
these files to the folder.

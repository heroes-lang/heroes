# Notes

## a1
- Record name: `struct sock_path` is a struct tag, so I wrote `record SockPath tag sock_path`. Without `tag`, the record would have to be called `sock_path`.
- `path` is `char[108]`, so its type is `i8[108]`, as the spec requires for char. Using `u8` would be refused.
- Filling `path`: I built the record with `rest: zero`, which zeroes all of `path`, then wrote each byte of the text through `sp.path[i] @ ...`. The other choice is to name `path:` with a literal of exactly 108 elements (16 characters and 92 zeros). It gives the same bytes but is long and easy to get wrong.
- Byte conversion: `text[i]` is a `u8`, so I used `.to_i8().must()`. The text is ASCII, so the conversion can't fail.
- `const struct sock_path *` became `@p: SockPath lent`, a whole record lent by address. A parameter without `@` would pass the struct by value, which doesn't match C.
- The result is declared `i64`, which the spec allows ("may be wider"); `i32` would also be accepted. Expected output: `16`.

## a2
- The named union `via`: the spec doesn't say how its members are named. I declared one member, `fd: i32`, as a field of the record. The other choice is `handle: ptr`, or both. If clang wants the path `via.fd`, this may be refused, and the spec offers no syntax for that.
- Zeroing the union: I wrote `Tagged(kind: 2, rest: zero)` and named no union member. That zeroes all 8 bytes of the union. Writing `fd: 0` would zero only 4 of them; the bytes `handle` adds would be left unset.
- Pointer parameter: same as a1 (`@t: Tagged lent`, result `i64`). Expected output: `2`.

## b
- `pthread_mutex_t` can't be expressed as a field, so I used `record Counter tag counter partial` and named only `n`. The size stays C's.
- Construction: `Counter(n: 0, rest: zero)`, then `counter_init`. `Counter(rest: zero)` should be equivalent.
- I marked every pointer parameter `@ ... lent`. Without `lent`, C is taken to keep the place, and the spec ties that to the place not moving.
- Increment function: `bump(@c: Counter)`. Its `@` means copy-in/copy-out, so the mutex bytes may move between calls. It is only ever copied while unlocked, which works on glibc and macOS, but POSIX doesn't promise it. The alternative is to inline the lock/increment/unlock in `main` with no function. That keeps the mutex in place but ignores the task's "write a function".
- Return codes are checked with `assert ... == 0`. The other choice, `_ = counter_lock(@c)`, would silently ignore errors.
- Expected output: `3`.

context: yes. The system prompt said I am a Claude agent built on the Claude Agent SDK, and gave the working directory (a `.claude/worktrees/readers/r10` path) and the date (2026-10-14). My only tool could read files but not write them. No other instructions, rules, memories or repository status reached me.

# Notes

## a1.hero
- Record name: `SockPath tag sock_path`. Alternative: name the record `sock_path` with no tag; same C type, unusual Heroes name.
- `char` → `i8[108]`, as section 13 says. Alternatives: `u8[108]`, which the spec suggests would be refused as the wrong sign.
- Filling `path`: build with `rest: zero` (all bytes zero, `path` included), then write the 16 bytes one by one through the cell (`u8` → `i8` via `to_i8().must()`). Alternative: a 108-element literal `[47, 116, ...]`, which the spec explicitly allows; same bytes, unreadable. Using a dynamic `[i8]` for the field is not allowed (a field is never a `[T]`).
- Pointer parameter: `@p ... lent` (C receives the place; `lent` because C doesn't keep it). Without `lent` the lend would be refused / C would be assumed to keep it; without `@` the struct would be passed by value, which doesn't match `const struct sock_path *`.
- Result `i32` (C's int); `i64` is also allowed ("a result may be wider"). Same output either way.
- Expected output: `16`.

## a2.hero
- The named union `via` is written as its members `fd: i32` and `handle: ptr` at the record's top level, since a field cannot be written as `via.fd`. Alternative: declare only one member (allowed: "one or more of each union").
- Built with `Tagged(kind: 2, rest: zero)`, naming no union member, so `via` is all zero. Alternative: `fd: 0` without `rest: zero` (naming exactly one) — also zero in `fd`, but on 64-bit the upper 4 bytes of `handle` would not be guaranteed zero.
- Pointer parameter `@t ... lent`, result `i32` — same reasons as A1.
- Expected output: `2`.

## b.hero
- `pthread_mutex_t` cannot be written as a field (opaque union typedef), so `record Counter tag counter partial` names only `n`; the size stays C's. Alternative: none that the spec offers.
- No `link "pthread"`: the task says nothing needs linking (modern glibc has pthread in libc). On older systems the alternative `link "pthread"` would be needed.
- Return codes are checked with `assert ... == 0`. Alternative: `_ = counter_lock(@c)`, which silently ignores failures.
- Mutating `n` is done in Heroes (`c.n @ c.n + 1`), between lock and unlock.
- Caveat: `@` is copy-in/copy-out, so inside `increment` the mutex is a copy of main's, copied back afterwards. It's always copied while unlocked, so glibc behaves, but POSIX leaves using a copied mutex undefined. The spec offers no way to pass the original place through a Heroes function. Alternative: inline lock/increment/unlock in `main` — violates the task.
- Expected output: `3`.

context: yes — an environment preamble (I am a Claude agent; a working-directory path showing a git worktree `.claude/worktrees/readers/r01` of `heroes-lang`; date 2026-10-14) and a toolset that could only read files, not write them, so these contents had to be given in the reply instead of saved. No other instructions, rules, memories or repository status.

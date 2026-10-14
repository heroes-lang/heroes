# Notes

## a1.hero
- Record name: the header writes `struct sock_path`, so `record SockPath tag sock_path`. Using `record sock_path` with no tag might also match, but the spec's examples use CamelCase with `tag`.
- `path` is `i8[108]` because the header says `char`. Declaring it `u8[108]` would be refused, since fields must match C's width and sign.
- Built with `family: 1, rest: zero`, then the 16 bytes written one by one into the cell. The other way is a 108-element array literal: same bytes, unreadable.
- `text[i]` is a `u8`, converted with `.to_i8().must()`. The text is ASCII, so this can't abort.
- `const struct sock_path *` is declared `@p: SockPath lent`. Without `@` the record would go by value, which the header doesn't match. Without `lent` the call wouldn't be a lend: C would be taken to keep the place.
- The result is `i32`, the same as C (a wider `i64` was allowed). Expected output: `16`.

## a2.hero
- The spec doesn't say how to write a named union's members (`via.fd`) as fields. I flattened them as `fd: i32` and `handle: ptr`. The other option was a `partial` record with only `kind`. But then it's unclear whether `rest: zero` zeroes bytes the record doesn't declare, so `via` might not be zero.
- I named both union members. Naming just one is allowed ("one or more of each union") and gives the same result.
- Built naming neither union member, ending with `rest: zero`, so all 8 union bytes are zero. Naming `fd: 0` instead would zero only 4 bytes; the rest would depend on `rest: zero` and the spec's sharing rule.
- Pointer parameter as `@t ... lent`, as in A1. Expected output: `2`.

## b.hero
- `pthread_mutex_t` can't be written as a field, so `record Counter tag counter partial` with only `n`. The size stays C's. Declaring every field isn't possible.
- Built with `n: 0, rest: zero` before `counter_init`, so no byte is undefined. Leaving out `rest: zero` might leave the bytes undefined; `counter_init` sets them anyway.
- `bump(@c: Counter)` is copy-in/copy-out, so the mutex bytes move into the parameter and back. Lock and unlock both act on the same place inside `bump`, so the mutex is never moved while locked. Copying an initialised mutex is still not allowed by POSIX (works in practice with glibc). The spec has no reference type to avoid this, and the task asks for a separate function.
- Every return code is bound and checked with `assert ... == 0`. The other option, `_ = counter_lock(@c)`, silently ignores failures.
- No `link "pthread"`: the task says nothing else needs linking. On an older glibc, pthread may need to be linked.
- All four C functions take the counter with `@... lent`. Expected output: `3`.

context: yes. The agent harness system prompt reached me: agent identity (Claude Agent SDK), the working directory (a git worktree path, `.claude/worktrees/readers/r02`), the date (2026-10-14), and tool-use instructions. The session also gave me only a read tool, so I couldn't write these files myself. No repository status, memory, or other rules reached me.

# Notes

## a1
- Pointer parameter: `@p: SockPath lent`, so C gets the place for this call only. Without `@`, the
  record would be passed by value and would not match C's `const struct sock_path *`. Without
  `lent`, C would be taken to keep the place.
- Record name: `SockPath tag sock_path`. Naming the record `sock_path` with no tag would also work.
- Filling `path`: built with `rest: zero` (which zeroes `path` and the padding), then the 16 bytes
  written one at a time from the string, converted u8 -> i8 with `to_i8(...).must()`. The other way
  is a 108-element `[...]` literal; it gives the same bytes but is long and easy to get wrong.
- `char` is `i8`, as section 13 requires. `u8` would be refused.
- Result is declared `i64` (wider than C's int is allowed); `i32` would also work. Expected output: 16.

## a2
- The union `via`: the record is `partial` and names only `kind`. `rest: zero` zeroes `via`, which
  shares no bytes with `kind`. The other way lists the union's members (`fd: i32`, `handle: ptr`) as
  fields. That may be refused because they are `via.fd` and `via.handle` in C, not fields of
  `tagged`, and the field syntax has no dotted names. If accepted, it would build the same bytes.
- Pointer parameter: `@t: Tagged lent`, for the same reason as in a1. Expected output: 2.

## b
- `pthread_mutex_t` can't be written as a field, so `Counter` is `partial` with only `n`. Its size
  stays C's, and `rest: zero` zeroes the mutex before `counter_init` runs.
- All four C functions are declared `lent` (none keeps the pointer after the call). If they were
  not `lent`, C would be taken to keep the place, and the program would then break the rule that
  the place must not move.
- Risk: `increment(@c)` copies in and copies out (section 9), so the mutex bytes move between
  calls. The mutex is always unlocked when it moves, and lock and unlock happen on the same place
  inside one call. Still, POSIX leaves the use of a copied mutex undefined. Nothing in the
  specification lets the function work on main's cell in place, because there are no references,
  globals or closures. The only way out is to do the locking inline in `main`, which the task rules out.
- The int return codes are checked with `assert ... == 0`. The other ways are `_ = counter_lock(@c)`
  (ignores failures) or `if rc != 0` with `exit(code: 1)`.
- No `link "pthread"` is written, because the task says nothing else needs linking. On an old glibc
  it might be needed. Expected output: 3.

## context
context: yes. The system prompt said I am a Claude agent built on the Claude Agent SDK, gave the
working directory (a git worktree path `.../.claude/worktrees/readers/r08`) and the date
(2026-10-14). My only tool could read files and not write them, so these four files were given in
my reply rather than written to disk. No repository status or memory reached me.
